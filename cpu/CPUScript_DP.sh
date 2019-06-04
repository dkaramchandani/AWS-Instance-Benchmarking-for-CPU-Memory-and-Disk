#!/bin/sh

echo CPU Benchmarking begins

make all

for i in 4 5 6 
	do
		for thread in 4 5 6
			do			
				sbatch run$((i)).slurm
			done
	done

echo CPU Benchmarking completed 
