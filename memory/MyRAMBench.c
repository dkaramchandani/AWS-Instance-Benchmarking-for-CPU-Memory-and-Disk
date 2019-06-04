#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

double throughput_latency,theoretical_val,efficiency_val;
long int blksize,operation_counter;

long int workload = 1024 * 1024 * 1024; //1 GB workload
long int loop_counter;
int numofthreads;	

void *seq();
void *rand_01();
void run_seq_access(int nthreads);
void run_rand_access(int nthreads);

struct timeval st,et;
double start_time, end_time, total_time;


//FILL ME IN
int main(int argc, char *argv[]) {
	
	
 	char blkname[5];
	char operation_name[4];
	int file_row[3];
	FILE  *fp;
	char output_file[100];
	//Reading from input_file for parameters
	if(argc == 2){
		 //= atoi(argv[1]);
		//printf("%s", input_file);
		fp = fopen(argv[1], "r" );
	}
	else{
		printf("Incorrect number of arguments\n");
	}
	
	
	if ( fp == NULL ) {
		// error handling..
		printf("Error! opening file");
	}
	fgets(operation_name, sizeof(operation_name),fp);
	printf("%s \n", operation_name);
	int i =0;
	
		
	while ( fscanf(fp, "%d",&file_row[i]) == 1 ) {
		printf("%d \n",file_row[i]);
		i++;
	}
	fclose(fp);
	
	//printf("%s",operation_name);
	blksize = file_row[0];
	numofthreads = file_row[1];
	
	
	operation_counter = 100;
	
	switch(blksize){
		case 1 : 
			blksize = 1; //1B
			operation_counter = 1;
			loop_counter = 100000000; //100 Million
			strcpy(blkname, "1B");
			break;
		case 1000 : 
			blksize = 1024; // 1KB
			loop_counter = workload/blksize;
			strcpy(blkname, "1KB");
			break;
		case 1000000 : 
			blksize = 1024 * 1024; //1MB
			loop_counter = workload/blksize;
			strcpy(blkname, "1MB");
			break;
		case 10000000 :
			blksize = 1024 * 1024* 10; //10MB
			loop_counter = workload/blksize;
			strcpy(blkname, "10MB");
			break;
		default: 
			printf("Invalid block size in the file\n");
	}
	
	printf("Operation: %s No of Threads: %d Blocksize: %ld Loop_counter: %ld \n", operation_name, numofthreads,blksize,loop_counter);
	
	//Running appropriate loop based on the operation_name from the input_file
	if(strncmp(operation_name,"RWR",3)==0){
		
		run_rand_access(numofthreads);
	}
	else if (strncmp(operation_name, "RWS",3)==0){
		run_seq_access(numofthreads);
	}
	else{
		printf("There is some issue with operation given in the input file!!");
	}
	
	
	
	//Calculate theoretical value of benchmark
	
	if(blksize == 1){
	//theoretical latency = clock cycle time ( in nanosec) x number of clock cycles ( also referred as CL)
	//theoretical latency = ((1/(Ram speed in GHz/2 * 1000)) * CL)/ 1000 -- since we need metrics in microsecs
	  theoretical_val = 0.01406;
	}
	else {
	    
	//theoretical_val = (Clock Freq)*(lines per clock)* (Width) * (# of Interfaces)
	//theoretical_val = ((2133 * 10^6) * 2 * (64/8) * 2)/(1024*1024*1024) -- to get metrics in GB/sec
		theoretical_val = 63.568354;
	}
	
	//Calculate efficiency value
	efficiency_val = (throughput_latency/theoretical_val)*100;
	
	//Creating output file name
	sprintf(output_file,"./output/memory-%s-%d-%dthread.out.dat",operation_name,file_row[0],numofthreads);
	printf("%s\n", output_file);
	
	//Write output to output file
	fp = fopen(output_file,"a");
	if ( fp == NULL ) {
		// error handling..
		printf("Error! opening file");
	}
	else {
		
		fprintf(fp,"%s %d %s %lf %lf %lf\n",operation_name,numofthreads,blkname,throughput_latency,theoretical_val,efficiency_val);
	}
	
	return 0;
}

void run_seq_access(int nthreads)
{
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
		
	printf("Inside 1st loop\n");
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	
	//creation of threads
	for(i=0;i<nthreads;i++){
		printf("Inside create loop\n");
		pthread_create(&arrayofthreads[i],NULL,seq,NULL);
	}
	

	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	//recalculate this
	if(blksize != 1)
	{
		throughput_latency = ((workload*100)/(total_time))/(1024*1024*1024); //since workload is 1GB hence we get metric in GB/sec 
		printf("Start time: %lf End Time: %lf Total Time: %lf sec\n",start_time,end_time,total_time);
		printf("Performed Sequential access in time %lf with Throughput @ %lf GB/sec\n", total_time, throughput_latency);
	}
	else {
		printf("Start time: %lf End Time: %lf Total Time: %lf sec\n",start_time,end_time,total_time);
		throughput_latency = ((total_time*1000000)/(workload/10));
		printf("Performed Sequential access in time %lf  with Latency @ %lf microsecs\n", total_time, throughput_latency);
	}	

	free(arrayofthreads);
}

void run_rand_access(int nthreads)
{
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);

	
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	//printf("Before thread create\n");
	//creation of threads
	for(i=0;i<nthreads;i++) {
		pthread_create(&arrayofthreads[0],NULL,rand_01,NULL);
	}

	
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	
	if(blksize != 1)
	{
		throughput_latency = ((workload*100)/(total_time))/(1024*1024*1024); //since workload is 1GB hence we get metric in GB/sec 
		printf("Start time: %lf End Time: %lf Total Time: %lf sec\n",start_time,end_time,total_time);
		printf("Performed Random access in time %lf with Throughput @ %lf GB/sec\n", total_time, throughput_latency);
		
	}
	else {
		printf("Start time: %lf End Time: %lf Total Time: %lf sec\n",start_time,end_time,total_time);
		throughput_latency = ((total_time*1000000)/(1024*1024*102.4));
		printf("Performed Random access in time %lf  with Latency @ %lf microsecs\n", total_time, throughput_latency);
	}	
	free(arrayofthreads);
}


void *seq()
{
	//int incrementby = 0;
	
	long int incrementby=0;
	
	int finish = loop_counter/numofthreads;
	printf("Finish %d", finish);
	
	//printf("finish %d\n",finish);
	char *workloadsizeblock = (char *) malloc((blksize*(finish))*sizeof(char));
	char *blksizeblock = (char *) malloc((blksize*(finish))*sizeof(char));
	
	memset(workloadsizeblock,'D',(blksize*(finish))*sizeof(char));
	//printf("After memset finish %d\n",finish);
	if (blksize == 1){
		
		printf("Inside blksize 1 loop\n");
		//printf("Loop_counter : %ld " , loop_counter);
		printf("Finish %d", finish);
		for(int counter = 0; counter < finish; counter++)
		{
			//printf("counter : %d", counter);
			memcpy(blksizeblock+incrementby,workloadsizeblock+incrementby,blksize);
			incrementby= incrementby + blksize;
		}
	}
	else
	{		
		incrementby = 0;
		for(long int x = 0; x < (finish*100) ; x++)
		{
			memcpy(blksizeblock+incrementby,workloadsizeblock+incrementby,blksize);
			incrementby= (x%finish==0)?0:(incrementby + blksize);
			
		}
	}

	free(workloadsizeblock);
	free(blksizeblock);
	
	//pthread_exit(NULL);
}

void *rand_01(void *nthread)
{
	printf("Inside random read\n");
	
	long int incrementby = 0;
	int finish = loop_counter/numofthreads;
	printf("Finish %d\n", finish);
	int randomlocation = 0;
	
	srand((unsigned)time(NULL));
	char *workloadsizeblock = (char *) malloc((blksize*(finish))*sizeof(char));
	char *blksizeblock = (char *) malloc((blksize*(finish))*sizeof(char));
	
	long int base4random = finish;
	printf("Before for, base4random %ld\n", base4random);
	
	//bzero(workloadsizeblock,(blksize*(finish))*sizeof(char));
	memset(workloadsizeblock,'D',(blksize*(finish))*sizeof(char));
	if (blksize == 1){
		
		printf("Inside blksize 1 loop\n");
		//printf("Loop_counter : %ld " , loop_counter);
		printf("Finish %d", finish);
	
	
		for(int counter = 0; counter < finish ; counter++)
		{
			//printf("Counter: %d\t", counter);
			randomlocation = abs(finish - rand()% base4random);
			memcpy(blksizeblock+randomlocation,workloadsizeblock+randomlocation,blksize);
			incrementby= (counter%finish==0)?0:(incrementby + blksize);
		}
	}
	else
	{
		incrementby = 0;
		for(long int x = 0; x < (finish*100) ; x++)
		{
			randomlocation = abs(finish - rand()% base4random);
			memcpy(blksizeblock+randomlocation,workloadsizeblock+randomlocation,blksize);
			incrementby= (x%finish==0)?0:(incrementby + blksize);
			
			/*if(x%100==0)
			{
				printf("Rndloc value : %d, blocksize : %ld\n", randomlocation, blksize);  
			}*/
		}
	}
	//printf("\n");
	
	free(workloadsizeblock);
	free(blksizeblock);
	
	//pthread_exit(NULL);
}
