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
#include <omp.h>

/*
 * 
 */
int main(int argc, char** argv) {
#pragma omp parallel
    {
        int ID = 0;
        printf("hello(%d)", ID);
        printf("world(%d)\n", ID);

    }
    return (EXIT_SUCCESS);
}

