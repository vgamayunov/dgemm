#!/bin/bash
# Example how to run with correct thread pinning.

NP=$(grep processor /proc/cpuinfo|wc -l)

# ICPC + MKL
OMP_NUM_THREADS=$NP OMP_PLACES="{0}:$NP" ./dgemm-mkl-icpc

# GCC + BLIS
OMP_NUM_THREADS=$NP OMP_PLACES="{0}:$NP" ./dgemm-blis-gcc
OMP_NUM_THREADS=$NP OMP_PLACES=cores ./dgemm-blis-gcc

# AOCC + BLIS
OMP_NUM_THREADS=$NP OMP_PLACES=nonsense ./dgemm-blis-aocc

# ICPC + BLIS
OMP_NUM_THREADS=$NP KMP_AFFINITY=physical ./dgemm-blis-icpc
