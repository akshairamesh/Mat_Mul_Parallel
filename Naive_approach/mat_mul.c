
// C program to multiply two square matrices.
#include <stdio.h>
#include<stdlib.h>
#include<limits.h>

#include<time.h>

#define LWB_DIM 1024
#define UPB_DIM 1536

// This function multiplies mat1[][] and mat2[][],
// and stores the result in res[][]
void multiply(int m, int n, int p,double * A, double * B, double * C)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < p; j++)
        {
            C[i + j] = 0;
            for (k = 0; k < n; k++)
                C[i+j] += A[i+k]* B[k+j];
        }
    }
}

int main()
{
    double * A, * B, * C;
    int r_iter, c_iter, m, n, p;
    A = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
    B = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
    C = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);

    if ((A == NULL) || (B == NULL) || (C == NULL)) {
    printf("Out of Memory\n");
    exit(1);
    }

    //Initializing matrixes with random values.
  for (r_iter = 0; r_iter < UPB_DIM; r_iter++)
    for (c_iter = 0; c_iter < UPB_DIM; c_iter++)
    {   A[r_iter * UPB_DIM + c_iter] = rand();
        B[r_iter * UPB_DIM + c_iter] = rand();
    }

    for (m = LWB_DIM; m <= UPB_DIM; m += 256) {
    n = p = m;

    time_t time_start = time(NULL);
    multiply(m,n,p,A, B, C);
    time_t time_end = time(NULL);
    int time_taken = (time_end - time_start);

    printf("For %dX%d matrix multiplication, execution_time =%dsecs\n", m, m, time_taken);
    }

    return 0;
}
