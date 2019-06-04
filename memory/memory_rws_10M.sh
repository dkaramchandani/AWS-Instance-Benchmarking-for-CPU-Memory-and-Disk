#!/bin/bash

echo "Sequential Read and Write in progress for 1 byte to 1MB"

#make all

for i in `seq 22 24`;
		do
			sbatch run$i.slurm
		done
