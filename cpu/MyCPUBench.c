#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

//#define NUM_THREADS 32
//pthread_mutex_t mutex;


double ops,theoretical_val,efficiency_val;
int numofthreads;

void *intOps();
void *doubleOps();
void *charOps();
void *shortOps();
void run_char_ops(int nthreads);
void run_short_ops(int nthreads);
void run_int_ops(int nthreads);
void run_double_ops(int nthreads);

struct timeval st,et;

double start_time, end_time, total_time, forloop_time;
int i;
pthread_barrier_t mybarrier;


//FILL ME IN
int main(int argc, char *argv[]) {
	
	//char input_file[200];
	//add file input from argv here
	
	char row[255];
	char operation_name[4];
	int file_row[3];
	FILE  *fp;
	char output_file[100];
	//Reading from input_file for parameters
	int i =0;
	if(argc >= 2){
		//input_file[200] = atoi(argv[1]);
		//printf("%s", input_file);
		
		fp = fopen(argv[1],"r");
	}
	else{
		printf("Incorrect number of arguments\n");
	}
	//fp = fopen(input_file, "r" );
	if ( fp == NULL ) {
		// error handling..
		printf("Error! opening file");
	}
	//fgets(operation_name, sizeof(operation_name),fp);
	
	fscanf(fp,"%s",operation_name);
	printf("%s\n", operation_name);
	
	while ( fscanf(fp, "%d",&file_row[i]) == 1 ) {
		printf("%d \n",file_row[i]);
		i++;
	}
	fclose(fp);
	
	//printf("%s",operation_name);
	
	numofthreads = file_row[0];
	//run_int_ops(file_row[0]);
	
	pthread_barrier_init(&mybarrier, NULL, numofthreads+1);
	
	//Running appropriate loop based on the operation_name from the input_file
	if(strncmp(operation_name,"SP",1)==0){
		//printf("here1");
		run_int_ops(numofthreads);
		//theoretical_val = (sockets)*(Number of cores)* (Freq of CPU in GHz)* (Operations per cycle) 
		//socket = 2 cpu 1 clock cycle = 2.30 flops 16
		theoretical_val = 2 * 1 * 2.30 * 32;	
	}
	else if (strncmp(operation_name, "QP",1)==0){
		run_char_ops(numofthreads);
		//theoretical_val = (sockets)*(Number of cores)* (Freq of CPU in GHz)* (Operations per cycle) 
		//socket = 2 cpu 1 clock cycle = 2.30 flops 128
		theoretical_val = 2 * 1 * 2.30 * 128;
	}
	else if(strncmp(operation_name, "HP",1)==0){
		run_short_ops(numofthreads);
		//theoretical_val = (sockets)*(Number of cores)* (Freq of CPU in GHz)* (Operations per cycle) 
		//socket = 2 cpu 1 clock cycle = 2.30 flops 64
		theoretical_val = 2 * 1 * 2.30 * 64;
	}
	else if(strncmp(operation_name, "DP",1)==0){
		//printf("here0");
		run_double_ops(numofthreads);
		//theoretical_val = (sockets)*(Number of cores)* (Freq of CPU in GHz)* (Operations per cycle) 
		//socket = 2 cpu 1 clock cycle = 2.30 flops 16
		theoretical_val = 2 * 1 * 2.30 * 16;
	}
	else{
		printf("There is some issue with workload given in the input file!!");
	}
	

	//Calculate efficiency value
	efficiency_val = (ops/theoretical_val)*100;
	
	//Creating output file name
	sprintf(output_file,"./output/cpu_%s_%dthread.out.dat",operation_name,file_row[0]);
	printf("%s\n", output_file);
	
	//Write output to output file
	fp = fopen(output_file,"a");
	if ( fp == NULL ) {
		// error handling..
		printf("Error! opening file");
	}
	else {
		//fprintf(fp,"%d %10f %10f %10f \n",file_row[0], operationspersec, xyz, abc);
		fprintf(fp,"%s %d %lf %lf %lf\n",operation_name,file_row[0],ops,theoretical_val,efficiency_val);
	}
	return 0;
}

void run_int_ops(int nthreads)
{
	printf("Inside run_ops\n");
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	int i;
	int thread_id[nthreads];
	
	//creation of threads
	for(i=0;i<nthreads;i++){
		printf("Inside create %d\n", i);
		thread_id[i] = i +1;
		pthread_create(&arrayofthreads[i],NULL,intOps,&thread_id[i]);
	}	
	
	pthread_barrier_wait(&mybarrier);
	//Starting time
		gettimeofday(&st, NULL);
			printf("Started timer\n");  	
	pthread_barrier_wait(&mybarrier);
	//Finishing time
		gettimeofday(&et, NULL);
			printf("Ended timer\n");  
	
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	//account in 
	double total = (10ul * 10000000 * (10000/nthreads) + ((10000/numofthreads)*4))/1000000000;
	ops = ((total*nthreads)/(end_time-start_time));
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec\n",total_time);
	printf("Performed Integer Operation @ %lf GIOPS\n", ops);
	
	free(arrayofthreads);

}

void *intOps(void *nthread)
{
	int thread_id = *(int*)nthread;
	//Initalize the integer variable
	int n1 = 1; 
	int n2 = -2; 
	int n3 = 3; 
	
	int niterations = 10000000;
	int i =0;int j;
	printf("Inside intops\n"); 
	printf("Before barrier, thread %d\n", thread_id);
	pthread_barrier_wait(&mybarrier);
	printf("After barrier 1, thread %d\n", thread_id);
	for(j = 0;j< 10000/numofthreads;j++)
	{
		for(i = 0;i<niterations;i++)	
		{
			//Operations done in the for loop :
			//2 - Counter operations
			//2 - Condition check
			
			//addition
			n1 = n1+n2;		n2 = n2+n3;		
			n1 = (n1+n3)-n3;
			//2* 2- Addition operations*/
			
			//multiplication
			//n1 = (n1*n3)/n3;
			//3- Multiplication operations*/
			
			
			//Total 10 operations
		}
	}		
	pthread_barrier_wait(&mybarrier);
	printf("After barrier 2, thread %d\n", thread_id);
	pthread_exit(NULL);
}

void run_double_ops(int nthreads)
{
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	int thread_id[nthreads];
	
	
	//creation of threads
	for(i=0;i<nthreads;i++){
		thread_id[i] = i +1;
		pthread_create(&arrayofthreads[i],NULL,doubleOps,&thread_id[i]);
	}
	pthread_barrier_wait(&mybarrier);
	//Starting time
		gettimeofday(&st, NULL);
	
	pthread_barrier_wait(&mybarrier);
	//Finishing time
		gettimeofday(&et, NULL);
	
	
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	//total Time taken for Execution
	total_time = (end_time-start_time);
	
	//account in 
	double total = (10ul * 1000000 * (100000/nthreads) + ((100000/numofthreads)*4))/1000000000;
	ops = ((total*nthreads)/(end_time-start_time));
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec",total_time);
	printf("Performed Floating Operation @ %lf GFOPS\n", ops);
	
	free(arrayofthreads);

}

void *doubleOps(void *nthread)
{
	//Initialise the double variables
	double n1 = 0.23; 
	double n2 = 0.34; 
	double n3 = 1.1; 
	
	int niterations = 1000000;
	
	int thread_id = *(int*)nthread;
	
	pthread_barrier_wait(&mybarrier);
	int x2;
		
	for(int x2 = 0;x2<(100000/numofthreads);x2++)
	{
		for(int i = 0;i<(niterations/numofthreads);i++)
		{
			//Operations done in the for loop :
			//2 - Counter operations
			//2 - Condition check
			
			//addition
			n1 = n1+n2;	n2 = n2+n3;
			n1 = (n1+n3)-n3;	
			//4 - Addition operations*/
			
			//multiplication
			//n1 = (n1*n3)/n3;	
			//3- Multiplication operations
			
			//Total 10 operations
		}
	}
	pthread_barrier_wait(&mybarrier);
	//printf("After barrier, thread %d\n", thread_id);

	pthread_exit(NULL);
}

void run_short_ops(int nthreads)
{
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	int i,x2;
	//Starting time
	printf("For loop startted\n");
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	for(x2 = 0; x2 <1000000/nthreads; x2++)
	{
		for( i = 0; i < 1000000;i++)
		{
				//nothing here	
		}
	}
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	
	forloop_time = end_time - start_time;
	printf("For loop time : %lf", forloop_time);
	int thread_id[nthreads];
	

	printf("Before create\n");
	//creation of threads
	for(i=0;i<nthreads;i++) {
		thread_id[i] = i +1;
		pthread_create(&arrayofthreads[i],NULL,shortOps,&thread_id[i]);
	}
	
	pthread_barrier_wait(&mybarrier);
	//Starting time
	printf("Started time \n");
		gettimeofday(&st, NULL);

	pthread_barrier_wait(&mybarrier);
	//Finishing time
		gettimeofday(&et, NULL);
		printf("Ended time \n");

	
	
	for(i=0;i<nthreads;i++)
	{
		pthread_join(arrayofthreads[i],NULL);
	}
	
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	//total Time taken for Execution
	total_time = abs((end_time-start_time)- forloop_time);
	
	//account in 
	double total = (10ul * 1000000 * (100000/nthreads) + ((100000/numofthreads)*4))/1000000000;
	ops = ((total*nthreads)/(end_time-start_time));
	
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec",total_time);
	printf("Performed Short Operation @ %lf GSOPS\n", ops);
	
	free(arrayofthreads);

}

void *shortOps(void *nthread)
{
	//Initalize the integer variable
	printf("Inside short Ops\n");
	short n1 = 1; 
	short n2 = 2; 
	short n3 = 3; 
	
	int niterations = 1000000;
	
	int thread_id = *(int*)nthread;
	
	printf("Before barrier %d \n", thread_id);
	pthread_barrier_wait(&mybarrier);
	printf("after barrier 1 %d \n", thread_id);
	int x2;
	
	for(x2=0; x2 < 100000/numofthreads; x2++)
	{
		for(int i = 0;i<niterations;i++)
		{
			//Operations done in the for loop :
			//2 - Counter operations
			//2 - Condition check
			
			//addition and subtraction
			n1 = n1+n2; n1 = n1-n2;
			n1 = (n1+n2)-n2;
			n2 = (n2+n3)-n3;
			
			//multiplication and division
			//n1 = (n1*n2)/n2;
			//n2 = (n2*n3)/n3;
			//2*2 + 3 
			//Total 10 operations
		}
	}		
	pthread_barrier_wait(&mybarrier);
	printf("After barrier 2, thread %d\n", thread_id);  
	
	pthread_exit(NULL);
}

void run_char_ops(int nthreads)
{
	//allocation of memory for threads
	pthread_t *arrayofthreads = malloc(sizeof(pthread_t)*nthreads);
	int x1, x2;
	//Starting time
	gettimeofday(&st, NULL);
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	/*for (x1 = 0 ; x1 < 100000/nthreads ; x1++)
	{
		for( x2 = 0; x2 < 10000000;x2++)
		{
				//nothing here	
		}
	}*/
	gettimeofday(&et, NULL);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	forloop_time = end_time - start_time;
	printf("For loop time : %lf", forloop_time);

	int thread_id[nthreads];
	//creation of threads
	for(i=0;i<nthreads;i++){
		thread_id[i]=i+1;
		pthread_create(&arrayofthreads[i],NULL,charOps,&thread_id[i]);
	}
	pthread_barrier_wait(&mybarrier);
	//Starting time
	gettimeofday(&st, NULL);
	pthread_barrier_wait(&mybarrier);
	//Finishing time
	gettimeofday(&et, NULL);
	
	
	for(i=0;i<nthreads;i++)
		pthread_join(arrayofthreads[i],NULL);
	
	
	start_time = st.tv_sec+(st.tv_usec/1000000.0);
	end_time = et.tv_sec+(et.tv_usec/1000000.0);
	//total Time taken for Execution
	total_time = abs((end_time-start_time)-forloop_time);
	
	//account in 
	double total = (10ul * 10000000 * (10000/nthreads) + ((10000/numofthreads)*4))/1000000000;
	ops = ((total*nthreads)/(end_time-start_time));
	
	printf("Start time : %lf and End time %lf\n", start_time, end_time);
	printf("Time Taken: %lf sec",total_time);
	printf("Performed Char Operation @ %lf GCOPS\n", ops);
	
	free(arrayofthreads);

}

void *charOps(void *nthread)
{
	//Initalize the integer variable
	char n1 = 'A'; 
	char n2 = 'B'; 
	char n3 = 'C'; 
	
	int niterations = 10000000;
	
	int thread_id = *(int*)nthread;
	int j;
	
	pthread_barrier_wait(&mybarrier);
	
	
	// total = 10 * 1trillon * (100/nthreads) + ((100/numofthreads)*4)
	for(int i = 0;i<10000/numofthreads;i++) 
	{
		//4 Operations
		for(j = 0; j < niterations ;j++)
		{
		//Operations done in the for loop :
		//2 - Counter operations
		//1 - Condition check
		
		n1 =  ((n1 - n2) + n2) + (n2 + n3) - n2 - n3;
		n1 =  ((n1 - n2) + n2);
		//7 - Addition & subtraction operations		
		//Total 10 operations
		}
		
	}
	pthread_barrier_wait(&mybarrier);
	printf("After barrier 2, thread %d\n", thread_id);  
	
	pthread_exit(NULL);
}