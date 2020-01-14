#!/bin/sh

module add mpi/intel/2018

echo "staring.."
date

mpicc -std=c99 $HOME/MPI/MPI_sort/MPI_sort.c -o $HOME/MPI/MPI_sort/MPI_sort -lm
mpirun $HOME/MPI/MPI_sort/MPI_sort

date