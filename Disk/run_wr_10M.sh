#!/bin/bash

echo "Write Random in progress for 100MB"

make all
for i in `seq 1 3`;
    do
	for i in `seq 47 49`;
        do
			sbatch run$i.slurm
        done
	done

