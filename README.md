# dgemm
Simple DGEMM with timings to be compiled with CBLAS library. Supports Intel MKL and AMD BLIS.

Examples how to build using different compilers and BLAS libraries are in build.sh.
Examples how to run are in run.sh.

Usage: 
```
./dgemm [-n matrix_size] [-r number_of_repetitions]
```
Defaults: -n 12000 -r 10
