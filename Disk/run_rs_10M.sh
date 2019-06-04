#!/bin/bash

echo "Read Sequential in progress for 10MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 21 23`;
        do
			sbatch run$i.slurm
        done
	done

