#include "mymalloc.h"

#define FALSE 0
#define TRUE 1
#define boolean char

static boolean memInit = 0;

typedef struct _MemoryData {
	char * startOfData;
	struct _MemoryData * next; 
	short int size;
	boolean isFree; 
}MemoryData; 

static char memoryblock[5000]; //Big block of memory
static MemoryData* mainMemory;
static MemoryData* firstFreeAddress; 

boolean initialize() {
	mainMemory = (MemoryData *)memoryBlock; //Creates a representation of main memory as a struct
        mainMemory->size = 5000 - sizeof(MemoryData); //The size of the memory that is available left for uses is this size    
	mainMemory->isFree = '1'; //
	mainMemory->startOfData = memoryblock;
	dataList = mainMemory; //Add to list. 
	return 1;
}

/*

int main(int argc, char * argv[]) {
	struct MemoryData * node;
	node = (struct MemoryData *) memoryblock; 

	node->size = 1239810239;
	node->next = NULL;
	node->arrayPtr = memoryBlock;

	printf("%d\n",node->size);
} 



*/


//This method will help with finding the first free 

MemoryData* findFirstFree(MemoryData * start, int size) {
	MemoryData * ptr = start;
	//Iterate through the memory blocks until you find a block that's both free and can fit in the memory we want to malloc, plus its metadata
	while ((start->isFree == FALSE || ptr->size < size + sizeof(MemoryData)) && ptr != NULL) {
		ptr = ptr->next;
	}
	return ptr;
}

void * mymalloc(int size, char* myfile, int line) {
	//If the attempted allocated size is 0, print error message and return 0
	
	if(size <= 0) { 
		printf("You have attempted to allocate a non-positive number bytes in File: '%s' Line: '%d'\n", myfile, line); 
		return 0;
	}

	if(memInit != 1) {
		initialize(); 
		firstFreeAddress = mainMemory; 
	} 

	if(size <= firstFreeAddress->size + sizeof(MemoryData)) { // This means that we have enough space in "main memory" to allocate
		//Set new memory location for free memory

		MemoryData* newFree = (MemoryData *)(firstFreeAddress->size + sizeof(MemoryData) + size); //Keeps track of free index
		newFree->size = mainMemory - sizeof(MemoryData) - size; //This keeps track of how much memory is free
		newFree->isFree = TRUE; 
		newFree->next = firstFreeAddress->next;			
			
		//Now I have to change the values for the data I allocated to something.
		firstFreeAddress->size = size;
		firstFreeAddress->isFree = FALSE; 
		firstFreeAddress->next = newFree;
		
		return (char*)firstFreeAddress;
 
	} else {
		printf("There is not enough space in memory in order to allocated the amount requested in File: '%s' Line: '%d'\n", myfile, line);
	}				 
}

void myfree(void * mementry, char * myfile, int line) {

}


