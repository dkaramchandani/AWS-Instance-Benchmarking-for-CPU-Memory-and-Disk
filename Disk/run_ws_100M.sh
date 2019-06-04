#!/bin/bash

echo "Write Sequential in progress for 100MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 33 35`;
        do
			sbatch run$i.slurm
        done
	done

