#!/bin/sh

echo CPU Benchmarking begins

make all

for i in 1 2 3
	do
		for j in 10 11 12
			do		
				
				sbatch run$j.slurm
			done
	done

echo CPU Benchmarking completed 
