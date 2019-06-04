#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

./MyDiskBench disk-RR-1-8thread.dat

