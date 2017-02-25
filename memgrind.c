#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

long getTimeElapsed(struct timeval begin, struct timeval end) {
	return (end.tv_sec - begin.tv_sec) * 1000000 + (end.tv_usec - begin.tv_usec);
}

long long int getAverageRunningTime(int (*memgrinder)()) {
	long long int totalTime = 0;
	struct timeval beginTime;
	struct timeval endTime;
	int counter = 0;
	while (counter < 10) {
		gettimeofday(&beginTime, 0);
		memgrinder();
		gettimeofday(&endTime, 0); 
		totalTime += getTimeElapsed(beginTime, endTime);
		counter++;
	}
	return (totalTime / 100);
}

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

int memgrind2() {
	int i = 0;
	while (i < 1000) {
		void* test = malloc(1);
		free(test);
		i++;
	}
	return 0;
}

int memgrind3() {
	int numMalloc = 0;
	int numFree = 0;
	void* testArray[1000];
	while (numMalloc < 1000) {
		if (rand() % 2 == 0) {
			testArray[numMalloc] = malloc(1);
			numMalloc++;
		} else {
			if (numFree < numMalloc) {
				free(testArray[numFree]);
				numFree++;
			}
		}
	}
	while (numFree < numMalloc) {
		free(testArray[numFree]);
		numFree++;
	}
	return 0;
}

int memgrind4() {
	int numMalloc = 0;
	int numFree = 0;
	int sizeMalloc = 0;
	void* testArray[1000];
	int sizeArray[1000];
	while (numMalloc < 1000) {
		if (rand() % 2 == 0 && sizeMalloc < memorySize) {
			int size = rand() % 64 + 1;
			testArray[numMalloc] = malloc(size);
			sizeArray[numMalloc] = size;
			sizeMalloc += size;
			numMalloc++;
		} else {
			if (numFree < numMalloc) {
				sizeMalloc -= sizeArray[numFree];
				free(testArray[numFree]);
				numFree++;
			}
		}
	}
	while (numFree < numMalloc) {
		free(testArray[numFree]);
		sizeMalloc -= sizeArray[numFree];
		numFree++;
	}
	return 0;
}

int memgrind5() {
	int * a = malloc(memorySize + 1);
	int * b = malloc(0);
	int * c = malloc(-3);
	free(NULL);
	free(-3);
	free("Test");
	char * x = malloc(1);
	free(&x);
	free(x + 10);
	*x = 'a';
	free(*x);
	return 0;
}

int memgrind6() { 
	void* mementries[1000];
	int index = 0; 
	int amountToMalloc = 1; 	

	while(index < 1000) {
		mementries[index] = malloc(amountToMalloc);
		
		if(mementries[index] == NULL) { // If NULL is returned, then there is no more space in memory which can fit. 
			break;	
		} else { 
			amountToMalloc = amountToMalloc * 2; 
			index++; 
		}
	}

	int max_index = index;
	index = 0; 

	printf("Index before 2nd loop is: %d\n", index);
	printf("Max Index before 2nd loop is %d\n", max_index); 

	while(index < max_index) {
		printf("Second loop index is: %d\n", index); 
		free(mementries[index]); 
		index = index + 2; 
	}
	
	index = 0;
	
	printf("Index before 3rd loop is: %d\n", index);
	printf("Max Index before 3rd loop is %d\n", max_index);

	while(index < 1000) {
		printf("Index in 3rd loop is: %d\n", index);

		mementries[index] = malloc(1);
		if(mementries[index] == NULL) { 
			break;
		}
		if(index >= max_index) {
			index ++;
		} else {
			index = index + 2; 
		}
	}
	
	max_index = index;		
	index = 0;
 
	printf("Index before 4th loop is: %d\n", index);
        printf("Max Index before 4th loop is %d\n", max_index);

	printf("%s\n", "4th LOOP!!!");
	
	while(index < max_index) {
		
		printf("Index is: %d\n", index);
		free(mementries[index]);
		index++; 
	}

	return 0;
}


int main(int argc, char *argv[]) {
//	printf("%lld\n", getAverageRunningTime(memgrind1));
	printf("%lld\n", getAverageRunningTime(memgrind2));
//	printf("%lld\n", getAverageRunningTime(memgrind3));
//	printf("%lld\n", getAverageRunningTime(memgrind4));
//	printf("%lld\n", getAverageRunningTime(memgrind5));
	return 0;
}

