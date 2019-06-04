#!/bin/bash

echo "Random Read and Write in progress for 10M"

#make all

for i in `seq 10 12`;
	do
		sbatch run$i.slurm
	done
