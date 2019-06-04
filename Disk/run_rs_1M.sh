#!/bin/bash

echo "Read Sequential in progress for 1MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 18 20`;
        do
			sbatch run$i.slurm
        done
	done

