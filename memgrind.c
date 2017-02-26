#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

// This function returns the number of microseconds between two timeval's.
long getTimeElapsed(struct timeval begin, struct timeval end) {
	return (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
}

// This function returns the average running time of a function over 100 executions.
long long int getAverageRunningTime(int (*memgrinder)()) {
	long long int totalTime = 0;
	struct timeval beginTime;
	struct timeval endTime;
	int counter = 0;
	while (counter < 100) {
		gettimeofday(&beginTime, 0);
		memgrinder();
		gettimeofday(&endTime, 0); 
		totalTime += getTimeElapsed(beginTime, endTime);
		counter++;
	}
	return (totalTime / 100);
}

// This function allocates 1 byte 1000 times, then frees them all one at a time.
int memgrind1() {
	void* testArray[1000];
	int i = 0;
	while (i < 1000) {
		testArray[i] = malloc(1);
		i++;
	}
	i = 0;
	while (i < 1000) {
		free(testArray[i]);
		i++;
	}
	return 0;
}

// This function allocates 1 byte and then frees it 1000 times.
int memgrind2() {
	int i = 0;
	while (i < 1000) {
		void* test = malloc(1);
		free(test);
		i++;
	}
	return 0;
}

/*
This function randomly either allocates 1 byte of memory or frees 1 byte of memory.
The number of free calls will never exceed the number of malloc calls and it will malloc up to 1000 times.
If there are still un-freed pointers at the end, they are all freed.
*/
int memgrind3() {
	// numMalloc keeps track of hoe many times malloc was called, while numFree keeps track of how many times free was called.
	int numMalloc = 0;
	int numFree = 0;
	void* testArray[1000];
	while (numMalloc < 1000) {
		// Either malloc 1 byte until 1000 are allocated in total or free one of the allocated 1 byte pointers.
		if (rand() % 2 == 0) {
			testArray[numMalloc] = malloc(1);
			if (testArray[numMalloc] != NULL) {
				numMalloc++;
			}
		} else {
			if (numFree < numMalloc) {
				free(testArray[numFree]);
				numFree++;
			}
		}
	}
	// At the end, if there are still any unfreed pointers, free them.
	while (numFree < numMalloc) {
		free(testArray[numFree]);
		numFree++;
	}
	return 0;
}

/*
This function randomly either allocated or frees 1 to 64 bytes.
It will never free if nothing is currently allocated, nor will it malloc if it will exceed total memory size.
If there are still un-freed pointers at the end, they are all freed.
*/
int memgrind4() {
	// numMalloc and numFree have the same functions as in memgrind3. sizeMalloc keeps track of how many bytes have been allocated.
	int numMalloc = 0;
	int numFree = 0;
	int sizeMalloc = 0;
	// testArray holds the pointers to whatever is malloced, while sizeArray holds the sizes of each malloc call.
	void* testArray[1000];
	int sizeArray[1000];
	while (numMalloc < 1000) {
		if (rand() % 2 == 0 && sizeMalloc < memorySize) {
			// The size allocated is a random number from 1 to 64 bytes.
			int size = rand() % 64 + 1;
			testArray[numMalloc] = malloc(size);
			if (testArray[numMalloc] != NULL) {
				// The size taken up in main memory is the size malloced plus the size of its metadata.
				sizeArray[numMalloc] = size + sizeof(MemoryData);
				sizeMalloc += size + sizeof(MemoryData);
				numMalloc++;
			}
		} else {
			if (numFree < numMalloc) {
				sizeMalloc -= sizeArray[numFree];
				free(testArray[numFree]);
				numFree++;
			}
		}
	}
	// At the end, free whatever's remaining.
	while (numFree < numMalloc) {
		free(testArray[numFree]);
		sizeMalloc -= sizeArray[numFree];
		numFree++;
	}
	return 0;
}

// This function tests malloc and free for invalid inputs.
int memgrind5() {
	// These malloc calls are too large, zero, and negative respectively.
	int * a = malloc(memorySize + 1);
	int * b = malloc(0);
	int * c = malloc(-3);
	// These free calls test freeing variables that can't be freed or have never been allocated.
	free(NULL);
	free(-3);
	free("Test");
	int * x = malloc(1);
	free(&x);
	free(x + 10);
	*x = 'a';
	free(*x);
	return 0;
}

/*
This function first allocates an increasing number of bytes in powers of two, then frees every other pointer. Then it fills up the
holes with malloc calls of 1 byte. Finally, everything is freed. This function tests whether malloc makes efficient use of every
empty space and how it handles spaces between memory blocks not large enough to hold metadata, e.g. 10 byte gaps.
*/
int memgrind6() { 
	void* mementries[1000];
	int index = 0; 
	int amountToMalloc = 1; 	

	while(index < 1000) {
		mementries[index] = malloc(amountToMalloc);
		// If NULL is returned, then there is no more space in memory which can fit. 
		if(mementries[index] == NULL) { 
			break;	
		} else { 
			// Increase the amount to malloc by a power of 2.
			amountToMalloc = amountToMalloc * 2; 
			index++; 
		}
	}

	int max_index = index;
	index = 0; 

	// Free every other pointer.
	while(index < max_index) {
		free(mementries[index]); 
		index = index + 2; 
	}
	
	index = 0;

	while(index < 1000) {

		mementries[index] = malloc(1);
		// If the malloc call failed because there wasn't enough memory, break.
		if(mementries[index] == NULL) { 
			break;
		}
		// Fill in every other pointer until index reaches max_index, then fill in every pointer.
		// This is to fill in every other pointer that was freed previously.
		if(index >= max_index) {
			index ++;
		} else {
			index = index + 2; 
		}
	}
	
	max_index = index;		
	index = 0;
	
	// Free all the pointers.
	while(index < max_index) {
		free(mementries[index]);
		index++; 
	}

	return 0;
}

int main(int argc, char *argv[]) {
	// Get the average running time for each of the memgrinds, then print the results at the end.
	long long int memgrind1time = getAverageRunningTime(memgrind1);
	long long int memgrind2time = getAverageRunningTime(memgrind2);
	long long int memgrind3time = getAverageRunningTime(memgrind3);
	long long int memgrind4time = getAverageRunningTime(memgrind4);
	long long int memgrind5time = getAverageRunningTime(memgrind5);
	long long int memgrind6time = getAverageRunningTime(memgrind6);
	printf("The average running time of memgrind1 was %lld microseconds.\n", memgrind1time);
	printf("The average running time of memgrind2 was %lld microseconds.\n", memgrind2time);
	printf("The average running time of memgrind3 was %lld microseconds.\n", memgrind3time);
	printf("The average running time of memgrind4 was %lld microseconds.\n", memgrind4time);
	printf("The average running time of memgrind5 was %lld microseconds.\n", memgrind5time);
	printf("The average running time of memgrind6 was %lld microseconds.\n", memgrind6time);
	return 0;
}
