###############BENCHMARKING#######################################	

This project aims at computing the benchmarks for CPU, Disk, Memory and Network on the Hyperion/Prometheus cluster.

CPU Benchmark: 
In this part of the assignment, we calculate the operations per second for payload of 1 trillion arthimetic operations for different precisions namely, Double, Single, Half and Quarter precisions with varying concurrency of 1,2 and 4 threads.
The code along with inputs and outputs are saved in the "cpu" folder of this git.

The input and output files follow the below naming convention with the output files being stored in the output folder.
cpu_<precision>_<numofthreads>thread.out.dat

Memory Benchmarking
In this part, we benchmark the throughput of the memory for a payload of 1 GB data run for over 100 times	with various access patterns namely Read Write Random and Read Write Sequential for block sizes 1KB, 1MB and 100 MB.
We also are calculating latency by doing 100 million read-write operations of 1 byte data.

Similar to cpu, the code along with the input and output files are present in the "memory" folder of this git.

The input and output files follow the below naming convention with the output files being stored in the output folder.
memory_<accesspattern>_<blocksize>_<numofthreads>thread.out.dat

Disk Benchmark:
This part of the assignment focuses on stressing the disk with a payload of 10GB data with various access patterns (Read Sequential, Write Sequential, Read Random and Write Random) for different block sizes (1MB, 10MB, 100MB). 
The latency of the disk is calculated by doing 100 million read-write operations of 1KB data. 

Similar to memory, the code along with the input and output files are present in the "disk" folder of this git.

The input and output files follow the below naming convention with the output files being stored in the output folder.
disk_<accesspattern>_<blocksize>_<numofthreads>thread.out.dat

Network Benchmark:
Lastly,we focus on benchmarking the network in this we are again working with a payload of 1 GB operating for 100 times with blocksizes of 1KB and 32KB to calculate the network's throughput.
We are also calculating the latency(ping-pong) for 1 million operations with 1 byte of data.
The benchmarking is done for both TCP as well as UDP protocol with  the code written to be executed on two different instances.

Similar to other parts of the assignment, the code along with the input and output files are present in the "network" folder of this git.

The input and output files follow the below naming convention with the output files being stored in the output folder.
network_<TCP/UDP>_<blocksize>_<numofthreads>thread.out.dat

##################Compilation of the code###########################
All the 4 benchmarks each have a different Makefile which contains the script to compile the code. Although the execution scripts have make file commands in them, if you wish to compile just the code by itself, you can do so using after navigating to the benchmark folder, opening a terminal and input the below :

make all

This would create compiled executable files in that folder.

Assuming that you have the git cloned or downloaded and extrated on the Linux testbed you want to test.

eg. To compile CPU, navigate to CPU folder, open a terminal and type make all as shown below.

dkaramchandani@bluecompute-1:~/./cs553-pa1/cpu$ make all

#########################How to run##################################

Assumptions : 
- This git is cloned or downloaded and extracted on the Linux testbed you want to test.
- The Linux testbed you have supports slurm based batch scheduling

Constraints: 
- Since there was a constraint limit of scheduling 10 jobs per user on Hyperion/Prometheus machines, hence all the scripts have been designed based on that view.

I. CPU Benchmark: 

1. Open the terminal for the instance you are running on.
2. Navigate to folder named "cpu"
3. Execute the script files named "CPUscript_<Precision>.sh" ,where Precision can be DP,SP,HP and QP,as shown below:
eg: srun CPUscript_SP.sh
4. The desired output of the program would be present in the "output" folder in the files :  'cpu_<Precision>_<numofthreads>thread.out.dat with numofthreads = 1,2 and 4.
eg: The output of the above run would be present in the output folder in the files :
cpu_SP_1thread.out
cpu_SP_2thread.out
cpu_SP_4thread.out

II. Memory Benchmark

1. Open the terminal for the instance you are running on.
2. Navigate to folder named "memory"
3. Execute the script files named "memory_<accesspattern>_<blocksize>.sh
where, accesspattern = rws or rwr
		blocksize = 1-1M or 10M
		
The script memory_<accesspattern>_1-1M will run all the tests for blocksizes 1B(latency), 1KB and 1MB.

eg: srun memory_rwr_10M.sh

4. The desired output of the program would be present in the "output" folder in the files :  'memory_<accesspattern>_<blocksize>_<numofthreads>thread.out.dat' 
where, 
accesspattern = rws or rwr
blocksize = 1 for 1B, 1000 for 1KB , 1000000 for 1MB and 10000000 for 10MB
and numofthreads = 1,2 and 4.
		
eg: The output of the above run would be present in the output folder in the files :
memory-RWR-10000000-1thread.out.dat
memory-RWR-10000000-2thread.out.dat
memory-RWR-10000000-4thread.out.dat

III. Disk Benchmark:
1. Open the terminal for the instance you are running on.
2. Navigate to folder named "disk"
3. Execute the script files named "disk_<accesspattern>_<blocksize>.sh
where,  accesspattern = rr , rs , ws and wr
		blocksize = 1K,1M,10M and 100M
All of the above will run for threads 1,2,4 with the exception of 1K script which will also run for threads 8,16,32,64 and 128		

eg: srun disk_ws_10M.sh

4. The desired output of the program would be present in the "output" folder in the files :  'disk_<accesspattern>_<blocksize>_<numofthreads>thread.out.dat' 
where, 
accesspattern = rs, ws, wr ,rr
blocksize = 1KB, 1000KB, 10000KB, 100000KB
and numofthreads = 1,2 and 4 for throughput and 1,2,4,8,16,32,128 for latency (1KB block size)

eg: The output of the above run for sequential write would be present in the output folder in the files :
disk-WS-100000-1thread.out.dat
disk-WS-100000-1thread.out.dat
disk-WS-100000-1thread.out.dat

IV. Network Benchmark
1. Open the terminal for the instance you are running on.
2. Navigate to folder named "network"
3. Execute the script files named "network_<mode>_<blocksize>.sh
where,  mode = TCP or UDP
		blocksize = 1B or 1K-32KB
	
All of the above will run for threads 1,2,4 and 8.

eg: srun disk_TCP_1B.sh

4. The desired output of the program would be present in the "output" folder in the files :  'network_<mode>_<blocksize>_<numofthreads>thread.out.dat' 
where, mode = TCP or UDP
blocksize = 1 for 1B, 1000 for 1KB and 32000 for 32KB
and numofthreads = 1,2, 4 and 8 

eg: The output of the above run for sequential write would be present in the output folder in the below files :
network-TCP-1-1thread.out
network-TCP-1-2thread.out
network-TCP-1-4thread.out
network-TCP-1-8thread.out

###########################SLURM Files###############################
In order to submit the jobs to slurm job, slurm files were made the  sample format of which is present below :
--sample.slurm-------------------------------------------------------
#!/bin/bash
#SBATCH --nodes=X
#SBATCH --ntasks-per-node=1

./<executable> <input dat file> <optional parameters>
--------------------------------------------------------------------

where, 
executable = Name of the object file of that benchmark
x = number of nodes needed to run.
Except for network benchmark, where x was 2, all other benchmarks had x = 1.

If you need to submit only particular job to the slurm, one can do by submitting "sbatch run(i).slurm" where (i) is to replaced by the number of the slurm file to be executed.

Sample of actual file:
-------------run1.slurm----------------------------------------------
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1

./MyCPUBench cpu_SP_1thread.dat
-------------------------------------------------------------------

###########################Benchmarking tools########################

Considering all benchmark files are present in the testbed, we can run them as below:
 
a.Linpack for CPU
	1. Download the binaries for Linpack from the below link : http://registrationcenter-download.intel.com/akdlm/irc_nas/9752/l_mklb_p_2018.2.010.tgz
	2. Extract and save them to a location, and then navigate to  "l_mklb_p_11.3.1.002/benchmarks_11.3.1/linux/mkl/benchmarks/linpack" path
	3. execute ./runme_xeon64 <br/>
		this will run the Benchmark with its own values and the output will be displayed
	4. to run with different input data, execute ./xlinpack_xeon64<br/>
		a message will be prompted press ENTER
		then type the values for
		Number of equations to solve (problem size):
		Leading dimension of array: (should be >= 5000)
		Number of trials to run:
		Data alignment value (in Kbytes): (should not be > 64)
	5. If your testbed supports slurm you can also submit the linpack(i).slurm files present in the benchmarks folder of this
	git.
b. IOZONE for Disk
	1. Navigate to http://www.iozone.org/src/current/iozone3_471.tar   to get the latest tarball and extract it to a location
	2. Navigate to '/iozone3_471/src/current/' path and using 
	   'make' command compile it 
	3. Once compiled, execute ./iozone -a
		This will give the Disk Benchmark values for different file sizes, in automatic mode.
	4. To execute for a particular file size, record size,threads
	   use the below command:
		execute  ./iozone -R -i 0 -i 1 -s 10g -r 1M -t 1 -F /tmp/g{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16} -u 1 -I 1 -O -I
		
		where /tmp/g{1,2,3,4,5,6,7,8,9,10} is the list of files that need to be created in '/tmp' folder 
		-l - u -t need to be set with number of threads to be used

	5. Since Hyperion/Prometheus both had IOZONE already compiled in to the testbed I was able to run it directly using the command in step4.

c. PMBW Benchmark for Memory
	1. Since PMBW is already installed in Hyperion/Prometheus, configuration and compilation was needed.
	2. Run the below commands to get benchmark results :
		pmbw -p 1 -P 4 -Q -s 1024 -S 10485760 -f ScanRead64PtrSimpleLoop
		pmbw -p 1 -P 4 -Q -s 1024 -S 10485760 -f PermRead64UnrollLoop
		pmbw -p 1 -P 4 -Q -s 1024 -S 10485760 -f ScanWrite64PtrUnrollLoop 
		
		where, -p = run the benchmark at the minimum thread count
			   -P = run the benchmark at maximum thread count
			   -s = run the benchmark at minimum record size
			   -S = run the benchmark at maximum record size
			   -Q = run the benchmark at quadratically increasing thread count
		So above command gives us the results for record sizes from 1KB to 10MB

d. IPERF for Network 
	1. do sudo apt-get install iperf, to install IPERF
	
	2. For testing network, we need to use 2 terminals,so as to run the server on one and client on other terminal.
	
	3. For TCP, on one terminal instance to run the Server execute: iperf3 -s and on the other terminal to run the Client execute: iperf3 -c <IPAddress of Server> -n <size of packet to send in bytes> - P <number of threads>
	4. It will send the packet and the bandwidth will be displayed accordingly
	
	4. For UDP, on one terminal instance to run the Server execute: iperf3 -su and
	 on other terminal to run the Client execute: iperf3 -c <IPAddress of Server> -u -n <size of packet to send in bytes> -P <number of threads>
	5. It will send the packet and the bandwidth will be displayed accordingly
	
	So, commands used : 
	On Server:
		For TCP - 
				iperf3 -s
		For UDP - 
				iperf3 -s
				
	On Client:
		For TCP - 
				iperf3 -c 127.0.0.1 -P 8
		For UDP - 
				iperf3 -c 127.0.0.45 -u -P 8