#!/bin/sh

echo CPU Benchmarking begins

make all

for i in 1 2 3
	do
		for j in 7 8 9
			do		
				
				sbatch run$j.slurm
			done
	done

echo CPU Benchmarking completed 
