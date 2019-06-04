#!/bin/bash

echo "Random Read in progress for 1 KB"

make all

for i in `seq 33`;
        do
			sbatch run$i.slurm
        done


