/**
 * CSE Lab project 2 -- C version
 * 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE	100
#define NUMBER_OF_THREADS	10
#define NELEMS(x)	(sizeof(x) / sizeof(x[0]))	// for calculating the number of elements in an array

void *get_max_from_array(void *params);	/* thread that performs basic maximun searching algorithm for subthread*/
void *get_max_from_result(void *params);	/* thread that performs maximun searching for final result */

int array[SIZE] = {};

int temp_result[NUMBER_OF_THREADS] = {};   /*store the temp maximum value for each thread*/

int maximun;                 /* final maximun value for whole array*/

typedef struct
{
	int start_index;
	int end_index;
	int temp_resultID;      // identify sub array ID
} parameters;

int main (int argc, const char * argv[])
{

    int i = 0;

    FILE *infile;
    infile = fopen("./input_1.txt", "r");

    while(!feof(infile))	// feof returns non-zero value if end-of-line indicator is set, else returns zero value
    {
		fscanf(infile,"%d",&array[i]);	// puts each number into a position with index i in the array
		//printf("number: %d, i: %d\n", array[i], i);
		i++;
    }

    fclose(infile);
    

    /* TODO: define threads array (tips: pthread_t) */
    pthread_t threadArray[NUMBER_OF_THREADS];	// create an array of threads

    int subArraySz = i/NUMBER_OF_THREADS;	// determine size of a sub-array, the sub-array here is not an actual array, just the no. of elements that each thread will search through

	
	/* TODO: create N threads */
	parameters *data[NUMBER_OF_THREADS]; 
	int idx;
	for (idx = 0; idx < NUMBER_OF_THREADS; idx++) {
		data[idx] = (parameters *) malloc (sizeof(parameters));

		if((i-1)%subArraySz == 0){
			data[idx]->start_index = idx*subArraySz;
			data[idx]->end_index = ((idx+1)*subArraySz)-1;
			data[idx]->temp_resultID = idx;
		}
		// In case SIZE/NUMBER_OF_THREADS is not an integer
		else{
			if(idx == NUMBER_OF_THREADS-1){
				data[idx]->start_index = idx*subArraySz;
				data[idx]->end_index = NELEMS(array)-1;
				data[idx]->temp_resultID = idx;
			}
			else{
				data[idx]->start_index = idx*subArraySz;
				data[idx]->end_index = ((idx+1)*subArraySz)-1;
				data[idx]->temp_resultID = idx;
			}
		}

		pthread_create(&threadArray[idx], NULL, get_max_from_array, data[idx]);
	}
	

	/* TODO: wait threads end */
	int ix;
	for(ix=0; ix<NUMBER_OF_THREADS; ix++){
		pthread_join(threadArray[ix], NULL);
	}

	// Print out the max no. from each thread
	int m;
	for(m=0; m<NELEMS(temp_result); m++){
		printf("Thread result: %d\n", temp_result[m]);
	}

	
	/* create new thread for final maximun searching */
	pthread_t findMax;
	parameters *data1;
	data1 = (parameters *) malloc(sizeof(parameters));
	data1->start_index = 0;
	data1->end_index = NUMBER_OF_THREADS-1;
	data1->temp_resultID = 0;
	pthread_create(&findMax, NULL, get_max_from_result, data1);
	
	/* wait for the final maximun search thread to finish */
	pthread_join(findMax, NULL);

	/* output the maximum value */
	printf("Maximun value is: %d \n", maximun);
	
    return 0;
}

/**
 * Maximum value search function.
 *
 * This thread can essentially use any algorithm for finding max value, or you can write by yourself
 * You can write any function format. such as write a general get_max function but not two seperate functions as below.
 */


/* write your get max value from origin array, will be used by multi-threads */
void *get_max_from_array(void *params) {
	/* TODO: add function content */
	parameters *indivData = (parameters *) params;

	int k;
	int max = 0;
	for(k=indivData->start_index; k<=indivData->end_index; k++){
		if(array[k] > max){
			max = array[k];
		}
	}
	temp_result[indivData->temp_resultID] = max;
	pthread_exit(NULL);
}


/* write your get max value from the new array which contains maximun value of all thread */
void *get_max_from_result(void *params) {
	/* TODO: add function content */
	parameters *finalData = (parameters *) params;

	int n;
	int max = 0;
	for(n=finalData->start_index; n<=finalData->end_index; n++){
		if(temp_result[n] > max){
			max = temp_result[n];
		}
	}
	maximun = max;
	pthread_exit(NULL);
}



