#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>

#if defined(MKL)
    #include "mkl.h"
    #define ALLOCATE(size) mkl_malloc(size, 64)
    #define FREE(p) mkl_free(p)
#endif

#if defined(BLIS)
    #include "blis/cblas.h"
#endif

#if ! defined(ALLOCATE)
  #define ALLOCATE(size) malloc(size)
#endif
#if ! defined(FREE)
  #define FREE(p) free(p)
#endif

#define min(x,y) (((x) < (y)) ? (x) : (y))

double seconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double) tv.tv_sec + (double) tv.tv_usec/1.0e6;
}

int main(int argc, char *argv[])
{
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;
    double time, flop, time1, mintime;
    int repeat;
    int c;
    int nt;

    m = n = k = 12000;
    repeat = 10;

    while ((c = getopt(argc, argv, "n:r:")) != -1)
        switch(c) {
            case 'n':
                m = n = k = strtol(optarg, NULL, 10);
                break;
            case 'r':
                repeat = strtol(optarg, NULL, 10);
                break;
        }

    nt = omp_get_max_threads();
    printf("DGEMM test. Size = %d, number of repetitions = %d, number of threads = %d\n", n, repeat, nt);

//    m = 2000, k = 200, n = 1000;
    alpha = 1.0; beta = 0.0;

// Allocating memory for matrices aligned on 64-byte boundary for better performance
    A = (double *)ALLOCATE(m * k * sizeof(double));
    B = (double *)ALLOCATE(k * n * sizeof(double));
    C = (double *)ALLOCATE(m * n * sizeof(double));
    if (A == NULL || B == NULL || C == NULL) {
      printf( "ERROR: Can't allocate memory for matrices. Aborting...\n");
      FREE(A);
      FREE(B);
      FREE(C);
      return 1;
    }

    printf ("Intializing matrix data...\n");
    for (i = 0; i < (m*k); i++) {
        A[i] = (double)(i+1);
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)(-i-1);
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf ("Running DGEMM ...\n");
    flop = 2.0 * m * n * k;
    time = seconds();
    for (i = 0; i < repeat; i++) {
        time1 = seconds();
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);
        time1 = seconds() - time1;
        printf("Time for run %2d was %.3lf sec, perf %.3lf GFlops\n", i, time1, flop/time1/1e9);
        if (i == 0)
            mintime = time1;
        else
            mintime = min(mintime, time1);
    }
    time = seconds() - time;
    printf ("DGEMM done in %.3lf seconds\n", time);
    printf("Average performance: %.3lf GFlops\n", repeat*flop/time/1e9);
    printf("Best performance:    %.3lf GFlops\n", flop/mintime/1e9);

    FREE(A);
    FREE(B);
    FREE(C);
    return 0;
}
