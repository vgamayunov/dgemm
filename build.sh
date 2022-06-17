#!/bin/bash

# Build MKL and BLIS versions using SPACK environment

BLIS_ROOT=$(spack location -i amdblis)

spack load gcc
spack load aocc
spack load intel-oneapi-compilers
spack load intel-mkl

set -x
# ICPC, MKL
icpc dgemm.c -DMKL -fopenmp -qmkl -o dgemm-mkl-icpc

# ICPC, BLIS
icpc dgemm.c -DBLIS -fopenmp -I $BLIS_ROOT/include -L $BLIS_ROOT/lib -lblis-mt -o dgemm-blis-icpc

# GCC, BLIS
gcc dgemm.c -DBLIS -fopenmp -I $BLIS_ROOT/include -L $BLIS_ROOT/lib -lblis-mt -o dgemm-blis-gcc

# AOCC, BLIS
clang dgemm.c -DBLIS -fopenmp -I $BLIS_ROOT/include -L $BLIS_ROOT/lib -lblis-mt -o dgemm-blis-aocc
