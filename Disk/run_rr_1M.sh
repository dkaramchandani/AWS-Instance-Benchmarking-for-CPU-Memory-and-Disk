#!/bin/bash

echo "Read Random in progress for 1MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 9 11`;
        do
			sbatch run$i.slurm
        done
	done

