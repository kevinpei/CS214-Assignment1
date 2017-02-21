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
	mainMemory = (MemoryData *)memoryblock; //Creates a representation of main memory as a struct
        mainMemory->size = 5000 - sizeof(MemoryData); //The size of the memory that is available left for uses is this size    
	mainMemory->isFree = '1'; //
	mainMemory->startOfData = memoryblock;

	return 1;
}

//This method will help with finding the first free 

MemoryData* findFirstFree(int size, MemoryData * start) {
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
	
	printf("%s\n", "Size is enough");

	if(memInit == FALSE) {
		printf("%s\n", "Initializing memory...");
		initialize(); 
		firstFreeAddress = mainMemory;
		memInit = TRUE;
	} else {
		firstFreeAddress = findFirstFree(size,firstFreeAddress); 
	}

	printf("%s\n", "Found first free address");
		
	if(firstFreeAddress != NULL) {  // This means that we have enough space in "main memory" to allocate
		//Set new memory location for free memory
		
		printf("%s\n", "Found address is not null...");
		
		MemoryData* newFree = (MemoryData *)(firstFreeAddress + 1 + (size/sizeof(MemoryData))); //Keeps track of free index
		printf("%d\n", firstFreeAddress);
		printf("%d\n", firstFreeAddress + 1);
		printf("%d\n", firstFreeAddress + 1 + (size/sizeof(MemoryData)));
		newFree->size = 5000 - sizeof(MemoryData) - size; //This keeps track of how much memory is free
		newFree->isFree = TRUE; 
		newFree->next = firstFreeAddress->next;			
		
		printf("%s\n", "Created new object and metadata...");
		
		//Now I have to change the values for the data I allocated to something.
		firstFreeAddress->size = size;
		firstFreeAddress->isFree = FALSE; 
		firstFreeAddress->next = newFree;

		printf("%s\n", "Pointing to new object and metadata");
			
		MemoryData test = *(firstFreeAddress);
	
		printf("%d\n", test.size);
		printf("%d\n", test.isFree); 
		
		printf("%s\n", "HOME STRETCH!!!");

		return firstFreeAddress + 1;
	} else {
		printf("There is not enough space in memory in order to allocated the amount requested in File: '%s' Line: '%d'\n", myfile, line);
		return NULL;
	}				 
}

void myfree(void * mementry, char * myfile, int line) {

}

