#ifndef MISC_H
# define MISC_H

#include <omp.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <time.h>

#include <omp.h>

#define LWB_DIM 1024 
#define UPB_DIM 1536
#define NUM_VALIDATES 10

void MatMul(int num_r1, int num_c1, int num_r2, int blocks, double const1, double const2, double * A, double * B, double * C);

#endif
