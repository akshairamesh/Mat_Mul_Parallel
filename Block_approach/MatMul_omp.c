#include <stdio.h>

#include "matrix.h"

#include<limits.h>

#include<time.h>
//Parallel computation of  C = const1 x A x B + const2 x C
// pickup each num_blocks X num_blocks block and store it in a temp.
// Use block multiplication algorithm to multiply the two matrices
// and store output in C. Add the temp scaled by factor of const2 to
// The newly computed C ( also scaled by factor of const1).
void MatMul(int m, int n, int p, int num_blocks, double const1, double const2, double * A, double * B, double * C) {
  int row_iter, col_iter, pivot, i, j, pivot_iter;
  double * Ctemp = (double * ) malloc(num_blocks * num_blocks * sizeof(double));

  #pragma omp parallel for private(row_iter, col_iter, pivot, i, j, pivot_iter) collapse(2)
  for (row_iter = 0; row_iter < m; row_iter += num_blocks) {
    for (col_iter = 0; col_iter < p; col_iter += num_blocks) {
      for (i = row_iter; i < row_iter + num_blocks; i++) {
        for (j = col_iter; j < col_iter + num_blocks; j++) {
          Ctemp[(i - row_iter) * num_blocks + (j - col_iter)] = C[i * p + j];
          C[i * p + j] = 0;
        }
      }
      for (pivot = 0; pivot < n; pivot += num_blocks) {
        //block multiply of Block_A[row_iter,pivot] * Block_B[pivot,col_iter] -> Block_C[row_iter,col_iter]
        for (i = row_iter; i < row_iter + num_blocks; i++) {
          for (j = col_iter; j < col_iter + num_blocks; j++) {
            for (pivot_iter = pivot; pivot_iter < pivot + num_blocks; pivot_iter++) {
              C[i * p + j] += A[i * n + pivot_iter] * B[pivot_iter * p + j];
            }
          }
        }
      }
      for (i = row_iter; i < row_iter + num_blocks; i++) {
        for (j = col_iter; j < col_iter + num_blocks; j++) {
          C[i * p + j] = const1 * C[i * p + j] + const2 * Ctemp[(i - row_iter) * num_blocks + (j - col_iter)];
        }
      }
    }
  }
}
