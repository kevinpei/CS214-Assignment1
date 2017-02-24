#include <stdio.h>
#include <stdlib.h>

int memgrind1() {
	void* testArray[1000];
	int i = 0;
	while (i < 1000) {
		testArray[i] = malloc(1);
		printf("%d\t", i);
		i++;
	}
	i = 0;
	while (i < 1000) {
		free(testArray[i]);
		printf("%d\t", i);
		i++;
	}
	return 0;
}

int memgrind2() {
	int i = 0;
	while (i < 1000) {
		void* test = malloc(1);
		free(test);
		printf("%d\t", i);
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
		if (rand() % 2 == 0 && sizeMalloc < 5000) {
			int size = rand() % 64 + 1;
			testArray[numMalloc] = malloc(size);
			sizeArray[numMalloc] = size;
			numMalloc++;
			sizeMalloc += size;
			printf("%d\t", sizeMalloc);
		} else {
			if (numFree < numMalloc) {
				sizeMalloc -= sizeArray[numFree];
				free(testArray[numFree]);
				numFree++;
				printf("%d\t", sizeMalloc);
			}
		}
	}
	while (numFree < numMalloc) {
		free(testArray[numFree]);
		numFree++;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	memgrind1();
	memgrind2();
	memgrind3();
	memgrind4();
	return 0;
}
