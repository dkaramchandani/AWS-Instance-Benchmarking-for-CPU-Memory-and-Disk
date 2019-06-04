#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

double throughput_latency,theoretical_val,efficiency_val,theoretical_iops,efficiency_val2,latency_iops;
long int blksize,operation_counter;
//change this when running on hyperion 
long int workload =  1024 * 1024 * 100; //10 GB workload
//long int workload = 1024*1024*1024*10ul; //10 GB workload

int numofthreads;
typedef struct thrdcounter {
	int initialoffset;
	int finaloffset;
	int thread_id;
} thrdcounter;

long int loop_counter;


void *seqread();
void *seqwrite();
void *randread();
void *randwrite();
void run_seq_read(int nthreads);
void run_seq_write(int nthreads);	
void run_rand_read(int nthreads);
void run_rand_write(int nthreads);

struct timeval st,et;

double start_time, end_time, total_time;

//FILL ME IN
int main(int argc, char **argv)
{
	//add file input from argv here
	char row[255];
	char operation_name[4];
	int file_row[3];
	FILE  *fp;
	char output_file[100];
	//Reading from input_file for parameters
	if(argc == 2){

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
	
	fscanf(fp, "%s",operation_name);
	
	//printf("%s\n", operation_name);
	int i =0;
	//if (fscanf(fp, "%d",file_row) == 1)
		
	while ( fscanf(fp, "%d",&file_row[i]) == 1 ) {
		printf("%d \n",file_row[i]);
		i++;
	}
	fclose(fp);
	
	
	blksize = file_row[0];
	numofthreads = file_row[1];
	printf("%s\n%d\n",operation_name,numofthreads);
	operation_counter = 100;
	
	switch(blksize){
		case 1 : 
			blksize = 1024; //1KB
			operation_counter = 1;

			//loop_counter = 1000000; //1 Million experiments
			loop_counter = 100000; //1 Million experiments
			break;
		case 1000 : 
			blksize = 1024*1024; // 1MB
			loop_counter = workload/blksize;
			break;
		case 10000 : 
			blksize = 1024 * 1024 * 10; //10MB
			loop_counter = workload/blksize;
			break;
		case 100000 :
			blksize = 1024 * 1024* 100; //100MB
			loop_counter = workload/blksize;
			break;
		default: 
			printf("Invalid block size in the file\n");
	}
	//Running appropriate loop based on the operation_name from the input_file
	if(strncmp(operation_name,"RR",2)==0){
		run_rand_read(numofthreads);
	}
	else if(strncmp(operation_name,"WR",2)==0){

		run_rand_write(numofthreads);
	}
	else if (strncmp(operation_name, "RS",2)==0){
		printf("Read\n");
		run_seq_read(numofthreads);
	}
	else if (strncmp(operation_name, "WS",2)==0){
		printf("Write\n");
		run_seq_write(numofthreads);
	}
	else{
		printf("There is some issue with operation given in the input file!!");
	}
	
	printf("Operation: %s No of Threads: %d Blocksize: %ld Loop_counter: %ld \n", operation_name, numofthreads,blksize,loop_counter);
	
	//Calculate theoretical value of Operation per sec
	//theoretical_val = (Freq of CPU in GHz)*(Number of cores)* (Instructions per cycle)
	if(blksize == 1024)
	{
		//this is latency
		theoretical_val = 4.16;
		latency_iops = 73.206442;	
		efficiency_val = (throughput_latency/theoretical_val);	
		efficiency_val2 = (theoretical_iops/latency_iops);
	}
	else {
	//Calculate efficiency value
	theoretical_val = 600;
	efficiency_val = (throughput_latency/theoretical_val)*100ul;
	}
	//Creating output file name
	sprintf(output_file,"./output/disk-%s-%d-%dthread.out.dat",operation_name,file_row[0],numofthreads);
	printf("%s\n", output_file);
	
	//Write output to output file
	fp = fopen(output_file,"a");
	if ( fp == NULL ) {
		// error handling..
		printf("Error! opening file");
	}
	else if(blksize != 1024) {
	
		fprintf(fp,"%s %d %ld %lf %lf %lf\n",operation_name,numofthreads,blksize,throughput_latency,theoretical_val,efficiency_val);
	}
	else {
		fprintf(fp,"%s %d %ld %lf %lf %lf %lf %lf %lf\n",operation_name,numofthreads,blksize,throughput_latency,theoretical_val,efficiency_val,theoretical_iops,latency_iops,efficiency_val2);
	}
	
	printf("Final values : %s %d %lf %lf %lf\n",operation_name,numofthreads,throughput_latency,theoretical_val,efficiency_val);
	
	return 0;
}
void run_seq_read(int nthreads){
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	thrdcounter thrdctr[nthreads];
	
	
	//creation of threads
	for(i=0;i<nthreads;i++) {
		
		pthread_create(&arrayofthreads[i],NULL,seqwrite,NULL);
	}
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	
	//creation of threads
	for(i=0;i<nthreads;i++){
		
		pthread_create(&arrayofthreads[i],NULL,seqread,NULL);
	}
		
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec\n",total_time);
		
	//recalculate this
	if(blksize != 1024)
	{
		//calculating throughput
 		//since workload is 1GB and we need thruput in MB hence dividing by 1024 *1024 to get metric in MB/sec 
		throughput_latency = ((workload)/(total_time))/(1024*1024);
		printf("Performed Sequential Read in time %lf with Throughput @ %lf MB/sec\n", total_time, throughput_latency);
	}
	else {
		//calculating latency
		
		throughput_latency = ((total_time*1000)/(workload)); // 10E3 is for microseconds conversion
		
		theoretical_iops = ((workload/blksize)/total_time);
		
		printf("Performed Sequential Read in time %lf  with Latency @ %lf microseconds and IOPS : %lf \n", total_time, throughput_latency,theoretical_iops);
	}	
	
	free(arrayofthreads);
}

void * seqread(void *nthread){
	printf("Seqreads k andar\n");
	int filepointer;
	
	
	long int incrementby = 0;
	int finish = (loop_counter/numofthreads);
	
	printf("Opening file\n");
	filepointer = open("/tmp/diskbenfile.dat", O_RDONLY | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	int counter = 0;
	int rc = 0;
	//for(int counter = 0; counter < operation_counter ; counter++)
	//{
		
		for(long int x = 0; x < finish ; x++)	
		{
			char *blksizeblock = (char *) malloc(blksize*sizeof(char));
			lseek(filepointer,incrementby,SEEK_SET);
			if((rc = read(filepointer, blksizeblock, blksize))==-1)
			{
				printf("Error writing file for iteration %ld", x);
				exit(0);
			}
			incrementby+=blksize;
			//incrementby= (x%finish==0)?0:(incrementby + blksize);
			counter+=rc;
			free(blksizeblock);
			fsync(filepointer);
		}
	//}
	//close file
	if(close(filepointer) == -1)
	{
		printf("Error in closing the file");
		exit(0);
	}
	printf("Num of times operation performed : %d\n", counter);
	
	//pthread_exit(NULL);
}

void run_seq_write(int nthreads){
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	thrdcounter thrdctr[nthreads];
	
	
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	printf("Create k pehele\n");
	//creation of threads
	for(i=0;i<nthreads;i++){
		
		pthread_create(&arrayofthreads[i],NULL,seqwrite,NULL);
	}
		
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec",total_time);
	
	
	//recalculate this
	if(blksize != 1024)
	{
		
 		//since workload is 1GB and we need thruput in MB hence dividing by 1024 *1024 to get metric in MB/sec 
		throughput_latency = ((workload)/(total_time))/(1024*1024);
		printf("Performed Sequential Write in time %lf with Throughput @ %lf MB/sec\n", total_time, throughput_latency);
	}
	else {
		//calculating latency
	
		throughput_latency = ((total_time*1000)/(workload)); // 10E3 is for microseconds conversion
	
		theoretical_iops = ((workload/blksize)/total_time);
		printf("Performed Sequential Write in time %lf  with Latency @ %lf microseconds and IOPS : %lf \n", total_time, throughput_latency,theoretical_iops);
	}	
		free(arrayofthreads);
}


void * seqwrite(void *nthread){
	
	printf("Seqwrite k andar\n");
	int filepointer;
	//int incrementby = 0;
	
	//char *workloadsizeblock = (char *) malloc(workload*sizeof(char));
	
	
	int finish = (loop_counter/numofthreads);
	int incrementby = 0;
	//Opening the file for sequential read
	printf("reached here2");
	if((filepointer = open("/tmp/diskbenfile.dat", O_WRONLY | O_CREAT | O_APPEND|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
	{
		printf("Error opening file 2\n");
		exit(0);
	}
	int counter = 0;
	int wc = 0;
	//for(int counter = 0; counter < operation_counter ; counter++)
	//{
		//for(long int x = begin; x <= finish ; x++)
		for(long int x = 0; x < finish ; x++)
		{
			char *blksizeblock = (char *) malloc(blksize*sizeof(char));
			memset(blksizeblock,'D', blksize);
			
			lseek(filepointer,0,SEEK_CUR); // lseek(filepointer,incrementby,SEEK_SET);
			
			if((wc = write(filepointer, blksizeblock, blksize))==-1)
			{
				printf("Error writing file for iteration %ld", x);
				exit(0);
				//incrementby= incrementby + blksize;
			}
			//incrementby+=blksize;
			counter+= wc;
			free(blksizeblock);
			fsync(filepointer);
		}
	//}
	if(close(filepointer) == -1)
	{
		printf("Error in closing the file");
		exit(0);
	}
	
	pthread_exit(NULL);
}

void run_rand_write(int nthreads){
	printf("randwrite k andar\n");
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	thrdcounter thrdctr[nthreads];
	
	//Starting time	
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	printf("create k pehele\n");
	//creation of threads
	for(i=0;i<nthreads;i++){
		//thrdctr[i].thread_id = i+1;
		//thrdctr[i].initialoffset = i * (loop_counter/numofthreads);
		//thrdctr[i].finaloffset=(i+1) * (loop_counter/numofthreads)-1;
		//pthread_create(&arrayofthreads[i],NULL,randwrite,&thrdctr[i]);
		pthread_create(&arrayofthreads[i],NULL,randwrite,NULL);
	}
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	//recalculate this
	if(blksize != 1024)
	{
		//calculating throughput
		
 		//since workload is 1GB and we need thruput in MB hence dividing by 1024 *1024 to get metric in MB/sec 
		throughput_latency = ((workload)/(total_time))/(1024*1024);
		printf("Performed Random Write in time %lf with Throughput @ %lf MB/sec\n", total_time, throughput_latency);
	}
	else {
		//calculating latency
		
		throughput_latency = ((total_time*1000)/(workload)); // 10E3 is for microseconds conversion
		
		theoretical_iops = ((workload/blksize)/total_time);
		printf("Performed Random Write in time %lf  with Latency @ %lf microseconds and IOPS : %lf\n", total_time, throughput_latency,theoretical_iops);
	}	
		
	free(arrayofthreads);
}

void * randwrite(void *nthread){
	
	printf("Asli jagah\n");
	int filepointer;
	int randomlocation= 0;
	
	/*thrdcounter *ctr1 = (thrdcounter *)nthread;
	
	int begin = ctr1->initialoffset; 
	printf("begin:%d\n",begin);
	int finish = ctr1->finaloffset;*/
	//printf("Finish:%d\n",finish);
	//int thread_id1 = ctr1->thread_id;
	
	//long int incrementby = begin * blksize;
	srand((unsigned)time(NULL));
	//char *workloadsizeblock = (char *) malloc(workload*sizeof(char));
	int finish = (loop_counter/numofthreads);
	
	//int base4random = finish-begin;
	int base4random = finish;
	printf("file k pehele Base4random : %d\n", base4random);
	//Opening the file for sequential read
	if((filepointer = open("/tmp/diskbenfile.dat", O_WRONLY | O_CREAT|O_TRUNC|O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
	{
		printf("Error opening file 3\n");
		exit(0);
	}
	int randompoint;
	//for(int counter = 0; counter < operation_counter ; counter++)
	//{
		//for(long int x = begin; x <= finish ; x++)
		for(long int x = 0; x <finish ; x++)
		{
			char *blksizeblock = (char *) malloc(blksize*sizeof(char));
			memset(blksizeblock,'D',blksize);
			randomlocation = abs(finish - (rand()% base4random));
			
			//move pointer to random position in file
			randompoint = lseek(filepointer,randomlocation,SEEK_SET); 
			if(randompoint == -1) {
				printf("Error in moving pointer");
			}
			
			if(write(filepointer, blksizeblock, blksize)==-1)
			{
				printf("Error writing file for iteration %ld", x);
				exit(0);
				//incrementby= incrementby + blksize;
			}
			free(blksizeblock);
			fsync(filepointer);
		}
	printf("file finished in randwrite\n");
	//}
	if(close(filepointer) == -1)
	{
		printf("Error in closing the file");
		exit(0);
	}
	printf("File closed\n");
	//pthread_exit(NULL);
}

void run_rand_read(int nthreads){
	int i;
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	pthread_t *arrayofthreads2 = malloc(sizeof(pthread_t)*nthreads);
	thrdcounter thrdctr[nthreads];
	
	//creation of threads
	for(i=0;i<nthreads;i++) {
		//thrdctr[i].thread_id = i+1;
		//thrdctr[i].initialoffset = i * (loop_counter/numofthreads);
		//thrdctr[i].finaloffset=(i+1) * (loop_counter/numofthreads)-1;
		//pthread_create(&arrayofthreads[0],NULL,seqwrite,&thrdctr[i]);
		pthread_create(&arrayofthreads[i],NULL,randwrite,NULL);
	}
	for(i=0;i<nthreads;i++){
		printf("Inside join\n, nthreads %d", nthreads);
		pthread_join(arrayofthreads[i],NULL);
		printf("join ends\n");
		
	}
	
	
	printf("wapas in randread\n");
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	
	
	//creation of threads
	for(i=0;i<nthreads;i++) {
		//thrdctr[i].thread_id = i+1;
		//thrdctr[i].initialoffset = i * (loop_counter/numofthreads);
		//thrdctr[i].finaloffset=(i+1) * (loop_counter/numofthreads)-1;
		//pthread_create(&arrayofthreads[i],NULL,randread,&thrdctr[i]);
		pthread_create(&arrayofthreads2[i],NULL,randread,NULL);
	}
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads2[i],NULL);
	
	//Finishing time
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	//recalculate this
	if(blksize != 1024)
	{
		//calculating throughput
		// is thruput = (blksize* loop_counter)/ (total_time * 1024 *1024);
		
 		//since workload is 1GB and we need thruput in MB hence dividing by 1024 *1024 to get metric in MB/sec 
		throughput_latency = ((workload)/(total_time))/(1024*1024);
		printf("Performed Random Read in time %lf with Throughput @ %lf MB/sec\n", total_time, throughput_latency);
	}
	else {
		//calculating latency
		//is latency = (total_time *1000)/loop_counter; // loop_counter is numofblocks 
		throughput_latency = ((total_time*1000)/(workload)); // 10E3 is for microseconds conversion
		//theoretical_iops = ((workload/total_time)/1e9);
		theoretical_iops = ((workload/blksize)/total_time);
		printf("Performed Random Read in time %lf  with Latency @ %lf microseconds and IOPS : %lf\n", total_time, throughput_latency,theoretical_iops);
	}	
	
	
	free(arrayofthreads);
	free(arrayofthreads2);
}


void * randread(void *nthread){
	
	printf("Inside random read\n");
	int filepointer,randomlocation;
	//int incrementby = 0;
	srand((unsigned)time(NULL));
	/*thrdcounter *ctr1 = (thrdcounter *) nthread;
	int begin = ctr1->initialoffset; 
	int finish = ctr1->finaloffset;
	int thread_id1 = ctr1->thread_id;*/
	int finish = (loop_counter/numofthreads);
	//long int incrementby = begin * blksize;
	long int incrementby = 0;
	
	//char *workloadsizeblock = (char *) malloc(workload*sizeof(char));
	//int base4random = finish-begin;
	int base4random = finish;
	printf("bEfore file open, finish %d and base4random : %d\n",finish,base4random);
	//Opening the file
	if((filepointer = open("/tmp/diskbenfile.dat", O_RDONLY| O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
	{
		printf("Error opening file 4\n");
		exit(0);
	}
	int randompoint;
	//for(int counter = 0; counter < operation_counter ; counter++)
	//{
		//for(long int x = begin; x <=finish ; x++)
	long int x;
		for(x = 0; x <finish ; x++)
		{
			char *blksizeblock = (char *) malloc(blksize*sizeof(char));
			
			randomlocation =(finish- (rand()% base4random));
			
			//move pointer to random position in file
			randompoint = lseek(filepointer,randomlocation,SEEK_SET); 
			if(randompoint == -1) {
				printf("Error in moving pointer");
			}
			
			if(read(filepointer, blksizeblock, blksize)==-1)
			{
				printf("Error reading file for iteration %ld", x);
				exit(0);
				//incrementby= incrementby + blksize;
			}
			free(blksizeblock);
			fsync(filepointer);
		}
		printf("after for loop, x %ld and base4random : %d\n",x,base4random);
	//}
	if(close(filepointer) == -1)
	{
		printf("Error in closing the file");
		exit(0);
	}
	//pthread_exit(NULL);
}
