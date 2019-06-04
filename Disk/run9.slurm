#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

./MyDiskBench disk-RR-1000-1thread.dat

