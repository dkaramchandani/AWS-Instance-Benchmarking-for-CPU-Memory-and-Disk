#!/bin/bash

echo "Random Read in progress for 1 KB"

make all

for i in `seq 1 8`;
        do
			sbatch run$i.slurm
        done


