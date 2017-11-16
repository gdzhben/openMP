/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: Ben
 *
 * Created on 12 November 2017, 20:53
 */
#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

struct timeval tv1, tv2;
struct timezone tz;

double multiply_matrix_get_norm(double *A, double *B, double *result, int n, int no_col)
{
        double first = 1.0, second = 0.0;
        ATL_dgemm(CblasNoTrans, CblasNoTrans,
                  n, no_col, n, first,
                  A, n,
                  B, n,
                  second,
                  result, n);

        int i, j;
        double max_sum = 0;
        for (i = 0; i < no_col; i++)
        {
                double sum = 0;
                for (j = 0; j < n; j++)
                {
                        sum += fabs(result[n * i + j]);
                }
                if (max_sum < sum)
                {
                        max_sum = sum;
                }
        }
        return max_sum;
}

double serial_multiply_matrix_get_norm(double *A, double *B, double *result, int n)
{
        double first = 1.0, second = 0.0;
        ATL_dgemm(CblasNoTrans, CblasNoTrans,
                  n, n, n, first,
                  A, n,
                  B, n,
                  second,
                  result, n);

        int i, j;
        double max_sum = 0;
        for (i = 0; i < n; i++)
        {
                double sum = 0;
                for (j = 0; j < n; j++)
                {
                        sum += fabs(result[n * i + j]);
                }
                if (max_sum < sum)
                {
                        max_sum = sum;
                }
        }
        return max_sum;
}

int main(int argc, char **argv)
{
        int n = 0;
        printf("Matrix n \t\t No.Of.Threads \t\t Serial Time \t\t Parallel Time\n");
        for (n = 100; n < 3000; n += 100)
        {
                double *A = (double *)malloc(n * n * sizeof(double));
                double *B = (double *)malloc(n * n * sizeof(double));
                double *result = (double *)malloc(n * n * sizeof(double));

                int i, j;
                for (i = 0; i < n * n; i++)
                {
                        A[i] = (double)(rand() % 1000);
                        B[i] = (double)(rand() % 1000);
                }

                double norm = 0.0;
                int no_of_threads = 8;


                gettimeofday(&tv1, &tz);
                omp_set_num_threads(no_of_threads);
#pragma omp parallel for shared(A, B, result, n, norm) private(i)
                for (i = 0; i < no_of_threads; i++)
                {
                        int partition = n / no_of_threads;
                        int col_no = i * partition;
                        if (i == no_of_threads - 1)
                        {
                                partition = n - (i * partition);
                        }

                        double sum = multiply_matrix_get_norm(A, &B[col_no * n],
                                                              &result[col_no * n], n, partition);

#pragma omp critical
                        {
                                if (norm < sum)
                                {
                                        norm = sum;
                                }
                        }
                }
                gettimeofday(&tv2, &tz);
                double time_parallel = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;


                gettimeofday(&tv1, &tz);

                double serious_sum = serial_multiply_matrix_get_norm(A, B, result, n);

                gettimeofday(&tv2, &tz);
                double time_serious = (double)(tv2.tv_sec - tv1.tv_sec) + (double)(tv2.tv_usec - tv1.tv_usec) * 1.e-6;

                printf("%d \t\t\t %d \t\t\t %f \t\t %f \n", n, no_of_threads, time_serious, time_parallel);
                free(A);
                free(B);
                free(result);
        }

        return (EXIT_SUCCESS);
}
