#!/bin/bash

# Example how to run with correct thread pinning.

# ICPC + MKL
OMP_NUM_THREADS=120 OMP_PLACES="{0}:120" ./dgemm-mkl-icpc

# GCC + BLIS
OMP_NUM_THREADS=120 OMP_PLACES="{0}:120" ./dgemm-blis-gcc
OMP_NUM_THREADS=120 OMP_PLACES=cores ./dgemm-blis-gcc

# AOCC + BLIS
OMP_NUM_THREADS=120 OMP_PLACES=nonsense ./dgemm-blis-aocc

# ICPC + BLIS
OMP_NUM_THREADS=120 KMP_AFFINITY=physical ./dgemm-blis-icpc
