#!/bin/bash

echo "Random Read and Write in progress for 1 byte to 1MB"

#make all

for i in `seq 1 9`;
		do
			sbatch run$i.slurm
		done
