#!/bin/bash

echo "Read Random in progress for 10MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 12 14`;
        do
			sbatch run$i.slurm
        done
	done

