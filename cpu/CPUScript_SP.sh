#!/bin/sh

echo CPU Benchmarking begins
listOfNames="SP"

make all

for operation in $listOfNames
do
	for i in 1 2 3
	do
		for t in 1 2 3
		do		
			#sbatch ./MyCPUBench cpu_${operation}_${thread}thread.dat	
			sbatch run$((t)).slurm
		done
	done
done

echo CPU Benchmarking completed 
