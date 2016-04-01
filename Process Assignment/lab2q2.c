/**
 * CSE lab project 2 -- C version
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define SIZE	500000
#define NUMBER_OF_THREADS	10
#define NELEMS(x)	(sizeof(x)/sizeof(x[0]))

void *sorter(void *params);	/* thread that performs sorting for subthread*/
void *merger(void *params);	/* thread that performs merging for final result */

int array[SIZE] = {};

// int temp_result[NUMBER_OF_THREADS][];   /*store the temp maximum value for each thread*/

int maximun;                 /* final maximun value for whole array*/

int merge_size;

int mergeArray[SIZE] = {};

typedef struct
{
	int start_index;
	int end_index;
} parameters;

typedef struct{
	int start_index;
	int end_index;
	int mid_index;
} parametersMid;


int main (int argc, const char * argv[]) 
{

    int i = 0;

    FILE *infile;
    infile = fopen("./input_2.txt", "r");

    while(!feof(infile))
    {
		fscanf(infile,"%d",&array[i]);
		i++;
    }

    fclose(infile);
    // printf("%d",array[0]);
    
	// pthread_t workers[NUMBER_OF_THREADS];

	// int subArraySz = i/NUMBER_OF_THREADS;
	// printf("i: %d\n", i);
 //    printf("subArraySz: %d\n", subArraySz);

    pthread_t partThread;

	
	clock_t start_clock, end_clock;
	start_clock = clock();
	/* establish the sorting thread */
	
	// parameters *data[NUMBER_OF_THREADS];
	// int idx;
	// for (idx = 0; idx < NUMBER_OF_THREADS; idx++) {
	// 	// TODO: create N sorting threads, notice the situation of SIZE/NUMBER_OF_THREADS being not integer
	// 	data[idx] = (parameters *) malloc (sizeof(parameters));

	// 	if((i-1)%subArraySz == 0){
	// 		data[idx]->start_index = idx*subArraySz;
	// 		data[idx]->end_index = ((idx+1)*subArraySz)-1;
	// 	}
	// 	// In case SIZE/NUMBER_OF_THREADS is not an integer
	// 	else{
	// 		if(idx == NUMBER_OF_THREADS-1){
	// 			data[idx]->start_index = idx*subArraySz;
	// 			data[idx]->end_index = NELEMS(array)-1;
	// 		}
	// 		else{
	// 			data[idx]->start_index = idx*subArraySz;
	// 			data[idx]->end_index = ((idx+1)*subArraySz)-1;
	// 		}
	// 	}

	// 	pthread_create(&workers[idx], NULL, sorter, data[idx]);

	// 	//printf("idx: %d, start_index: %d, end_index: %d\n", idx, data[idx]->start_index, data[idx]->end_index);
	// }

	parameters *partData;
	partData = (parameters *) malloc(sizeof(parameters));
	partData->start_index = 0;
	partData->end_index = NELEMS(array)-1;
	pthread_create(&partThread, NULL, sorter, partData);

	pthread_join(partThread, NULL);


	/* TODO: now wait for the sorting threads to finish, consider free memory */
	// int ix;
	// for(ix=0; ix<NUMBER_OF_THREADS; ix++){
	// 	pthread_join(workers[ix], NULL);
	// 	free(data[ix]);
	// }

	
	// int m;
	// for(m=0; m<NELEMS(array); m++){
	// 	printf("%d\n", array[m]);
	// }
	
	
	/* establish the final merge threads
	*  You can merge all the sorted arrays in couples, then deal the residual array. Then do this recursively
	*/


	// pthread_t mergeThread;
	// parameters *data1[NUMBER_OF_THREADS];
	// int p;
	// for (p = 0; p < NUMBER_OF_THREADS; p++) {
	// 	// TODO: create N sorting threads, notice the situation of SIZE/NUMBER_OF_THREADS being not integer
	// 	printf("SUP\n");

	// 	data1[p] = (parameters *) malloc (sizeof(parameters));

	// 	if((i-1)%subArraySz == 0){
	// 		data1[p]->start_index = p*subArraySz;
	// 		data1[p]->end_index = ((p+1)*subArraySz)-1;
	// 	}
	// 	// In case SIZE/NUMBER_OF_THREADS is not an integer
	// 	else{
	// 		if(p == NUMBER_OF_THREADS-1){
	// 			data1[p]->start_index = p*subArraySz;
	// 			data1[p]->end_index = NELEMS(array)-1;
	// 		}
	// 		else{
	// 			data1[p]->start_index = p*subArraySz;
	// 			data1[p]->end_index = ((p+1)*subArraySz)-1;
	// 		}
	// 	}

	// 	pthread_create(&mergeThread, NULL, merger, data1);

	// 	//printf("p: %d, start_index: %d, end_index: %d\n", p, data1[p]->start_index, data1[p]->end_index);
	// }

	//pthread_t mergeT;



	/* TODO: create threads for merge two sorted arrays (recursively) */
	
	
	/* TODO: deal with the odd number of sorted array */


	/* TODO: merge the final two array get the full sorted array */

	

	/* output the time consumption */
	end_clock = clock();
	float period_time = (float)(end_clock - start_clock) / CLOCKS_PER_SEC;
	printf ("Total time consumption: %.6f seconds \n", period_time);
	// print all value of sorted array
	// printf("Final result: \n");
	// for (i = 0; i < SIZE; i++) {
	// 	printf("%d  ",array[i]);
	// }
	printf("\n");
	
    return 0;
}

/**
 * Merger and sorter function.
 *
 * This thread can essentially use any algorithm, or you can write by yourself
 */

// sort an unordered array  ascendingly
void *sorter(void *params) {
	/* TODO: add function content */
	// parameters *indivData = (parameters *) params;

	// //int n = indivData->end_index;
	// printf("\n%s\n", "Entered thread!");
	// int c, d, swap;

	// for(c=indivData->start_index; c<=indivData->end_index-1; c++){
	// 	for(d=indivData->start_index; d<=indivData->end_index-1; d++){
	// 		if(array[d] > array[d+1]){
	// 			swap = array[d];
	// 			array[d] = array[d+1];
	// 			array[d+1] = swap;
	// 		}
	// 	}
	// }
	// printf("SWAPPED\n");
	// for(c=indivData->start_index; c<=indivData->end_index; c++){
	// 	printf("%d\n", array[c]);
	// }
	// printf("PRINTED\n");

	//printf("\n%s\n", "Entered sorter thread!");

	parameters *iPartData = (parameters *) params;
	int mid;
	//printf("iPartData start_index: %d, iPartData end_index: %d\n", iPartData->start_index, iPartData->end_index);
	if(iPartData->start_index < iPartData->end_index){
		//printf("\n%s\n", "Gonna create sorter!");
		mid = (iPartData->start_index + iPartData->end_index) / 2;
		
		pthread_t partThread_1;
		parameters *partData_1;
		partData_1 = (parameters *) malloc(sizeof(parameters));
		partData_1->start_index = iPartData->start_index;
		partData_1->end_index = mid;
		pthread_create(&partThread_1, NULL, sorter, partData_1);
		pthread_join(partThread_1, NULL);

		pthread_t partThread_2;
		parameters *partData_2;
		partData_2 = (parameters *) malloc(sizeof(parameters));
		partData_2->start_index = mid + 1;
		partData_2->end_index = iPartData->end_index;
		pthread_create(&partThread_2, NULL, sorter, partData_2);
		pthread_join(partThread_2, NULL);

		pthread_t mergeThread;
		parametersMid *mergeData;
		mergeData = (parametersMid *) malloc(sizeof(parametersMid));
		mergeData->start_index = iPartData->start_index;
		mergeData->end_index = iPartData->end_index;
		mergeData->mid_index = mid;
		pthread_create(&mergeThread, NULL, merger, mergeData);
		pthread_join(mergeThread, NULL);
	}


	pthread_exit(NULL);
}


// merge two sorted array to a new sorted array
void *merger(void *params) {
	/* TODO: add function content */
	// parameters[NUMBER_OF_THREADS] *indiData = (parameters *) params;

	// int t;
	// int q = indiData[0]->start_index;
	// int r = indiData[0]->end_index;
	// int subArraySz = (indiData[0]->end_index)-(indiData[0]->start_index);
	// int min;
	// int s = 0;
	// for(t=0; t<NELEMS(array); t++){
	// 	for(q=0; q<NELEMS(array)-subArraySz; q+subArraySz){
	// 		if(array[q] < array[q+subArraySz]){
	// 			min = array[q];
	// 		}
	// 	}
	// 	mergeArray[s] = min;
	// 	s++;
	// }
	
	// int u;
	// for(u=0; u<NELEMS(mergeArray); u++){
	// 	printf("%d\n", mergeArray[u]);
	// }

	//printf("\n%s\n", "Entered merger thread!");

	parametersMid *iMergeData = (parametersMid *) params;

	int tmp[SIZE];
	int i, j, k, m;
	j = iMergeData->start_index;
	m = iMergeData->mid_index + 1;
	for(i=iMergeData->start_index; j<=iMergeData->mid_index && m<=iMergeData->end_index; i++){
		if(array[j] <= array[m]){
			tmp[i] = array[j];
			j++;
		}
		else{
			tmp[i] = array[m];
			m++;
		}
	}
	if(j>iMergeData->mid_index){
		for(k=m; k<=iMergeData->end_index; k++){
			tmp[i] = array[k];
			i++;
		}
	}
	else{
		for(k=j; k<=iMergeData->mid_index; k++){
			tmp[i] = array[k];
			i++;
		}
	}
	for(k=iMergeData->start_index; k<=iMergeData->end_index; k++){
		array[k] = tmp[k];
	}

	pthread_exit(NULL);
}



