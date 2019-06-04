#!/bin/bash

echo "Write Random in progress for 1KB"

make all
for i in `seq 36 43`;
        do
			sbatch run$i.slurm
        done


