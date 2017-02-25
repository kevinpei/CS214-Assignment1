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

int main(int argc, char *argv[]) {
//	printf("%lld\n", getAverageRunningTime(memgrind1));
	printf("%lld\n", getAverageRunningTime(memgrind2));
//	printf("%lld\n", getAverageRunningTime(memgrind3));
//	printf("%lld\n", getAverageRunningTime(memgrind4));
//	printf("%lld\n", getAverageRunningTime(memgrind5));
	return 0;
}
