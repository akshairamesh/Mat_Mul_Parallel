#include <stdio.h>

#include "matrix.h"

#include<limits.h>

#include<time.h>

int main(int argc, char ** argv) {
  //Variable Declaration
  double * A, * B, * C, * Corg, const1, const2;
  int m, n, p, num_block, block_iter, block_min, block_max;
  int r_iter, c_iter, num_thread;
  int thread_count, block_count, min_time_taken;
  char status[100];
  //A,B are input matrixes of size m*n and n*p. C will be the resultant matrix
  A = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
  B = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
  C = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
  Corg = (double * ) malloc(sizeof(double) * UPB_DIM * UPB_DIM);
  if ((A == NULL) || (B == NULL) || (C == NULL) || (Corg == NULL)) {
    printf("Out of Memory\n");
    exit(1);
  }

  const1 = (double) 1.3;
  const2 = (double) 2.9;
  //Initializing matrixes with random values.
  for (r_iter = 0; r_iter < UPB_DIM; r_iter++)
    for (c_iter = 0; c_iter < UPB_DIM; c_iter++)
    {   A[r_iter * UPB_DIM + c_iter] = rand();
        B[r_iter * UPB_DIM + c_iter] = rand();
        Corg[r_iter * UPB_DIM + c_iter] = rand();
    }
  //Vary the dimension of matrixes from 1024x1024 to 1536x1536 in steps of 256.
  for (m = LWB_DIM; m <= UPB_DIM; m += 256) {
    min_time_taken = INT_MAX;
    n = p = m;
    //Vary the number of threads as num_thread = 1,2 and 4
    for (num_thread = 1; num_thread <= 4; num_thread *= 2) {
      if (num_thread == 4) {
        block_min = 4;
        block_max = 64;
      } else
        block_min = block_max = 16;
      //vary the number of blocks as num_blocks = 4,16,64
      for (block_iter = block_min; block_iter <= block_max; block_iter *= 4) {
        for (r_iter = 0; r_iter < m; r_iter++)
          for (c_iter = 0; c_iter < p; c_iter++)
            C[r_iter * p + c_iter] = Corg[r_iter * p + c_iter];
        num_block = block_iter;
        if (num_block > m) num_block = m;
        if (num_block > n) num_block = n;
        if (num_block > p) num_block = p;
        //assign number of threads for omp.
        omp_set_num_threads(num_thread);

        time_t time_start = time(NULL);
        //MatMul() is the function that implements the matrix multiplication using block algorithm.
        MatMul(m, n, p, num_block, const1, const2, A, B, C);
        time_t time_end = time(NULL);

        //validate the random entries.
        int success = 1, r;
        for (r = 0; r < NUM_VALIDATES; r++) {
          double min, max, err, ans;

          r_iter = (rand() % m);
          c_iter = (rand() % p);

          int k;
          ans = Corg[r_iter * p + c_iter] * const2;
          for (k = 0; k < n; k++)
            ans += const1 * A[r_iter * n + k] * B[k * p + c_iter];

          if (ans > C[r_iter * p + c_iter]) {
            min = C[r_iter * p + c_iter];
            max = ans;
          } else {
            min = ans;
            max = C[r_iter * p + c_iter];
          }
          err = (double)(max - min) / (double) max;

          if ((min > (double) 0.0001) && (err > (double) 0.01))
            success = 0;
        }

        if (success == 0)
          strcpy(status, "Failed");
        else
          strcpy(status, "Passed");
        //storing values for minimun execution time.
        int time_taken = (time_end - time_start);
        if (min_time_taken >= time_taken) {
          min_time_taken = time_taken;
          thread_count = num_thread;
          block_count = num_block;
        }
        printf("{m=%d,n=%d,p=%d,num_thread=%d,num_block=%d,time=%dsecs,status=%s}\n",
          m, n, p, num_thread, num_block, time_taken, status);
        fflush(stdout);
      }
    }
    printf("For %dX%d matrix multiplication, the min_exe_time=%dsecs, num_threads=%d, num_blocks=%d\n", m, m, min_time_taken, thread_count, block_count);

  }
  return 0;
}
