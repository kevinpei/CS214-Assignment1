#include "mymalloc.h"

#define FALSE 0
#define TRUE 1
#define boolean char

static boolean memInit = 0;

typedef struct _MemoryData {
	char * startOfData;
	struct _MemoryData * next;
	struct _MemoryData * prev; 
	short int size;
	boolean isFree; 
}MemoryData; 

static char memoryblock[5000]; //Big block of memory
static MemoryData* mainMemory;
static MemoryData* firstFreeAddress; 

boolean initialize() {
	mainMemory = (MemoryData *)memoryblock; //Creates a representation of main memory as a struct
    	mainMemory->size = 5000 - sizeof(MemoryData); //The size of the memory that is available left for uses is this size    
	mainMemory->isFree = TRUE; //
	mainMemory->startOfData = memoryblock;
	mainMemory->next = NULL;
	mainMemory->prev = NULL;
	return TRUE;
}

//This method will help with finding the first free 

MemoryData* findFirstFree(int size, MemoryData * start) {
	MemoryData * ptr = start;
	//Iterate through the memory blocks until you find a block that's both free and can fit in the memory we want to malloc, plus its metadata
	while ( ptr != NULL) {
		if (ptr->isFree == TRUE && ptr->size > size + sizeof(MemoryData)) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

void * mymalloc(int size, char* myfile, int line) {
	//If the attempted allocated size is 0, print error message and return 0
	
	if(size <= 0) { 
		printf("You have attempted to allocate a non-positive number bytes in File: '%s' Line: '%d'\n", myfile, line); 
		return FALSE;
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
		
		MemoryData* newFree = (MemoryData *)((char *)firstFreeAddress + sizeof(MemoryData) + size); //Keeps track of free index
		printf("%d\n", firstFreeAddress);
		printf("%d\n", sizeof(MemoryData) + size);
		printf("%d\n", (char *)firstFreeAddress + sizeof(MemoryData) + size);
		newFree->size = firstFreeAddress->size - sizeof(MemoryData) - size; //This keeps track of how much memory is free
		printf("%d\n", newFree->size);
		newFree->isFree = TRUE; 
		newFree->next = firstFreeAddress->next;	
		newFree->prev = firstFreeAddress->prev;
		
		printf("%s\n", "Created new object and metadata...");
		
		//Now I have to change the values for the data I allocated to something.
		firstFreeAddress->size = size;
		firstFreeAddress->isFree = FALSE; 
		firstFreeAddress->next = newFree;
		newFree->prev = firstFreeAddress;

		printf("%s\n", "Pointing to new object and metadata");
			
		MemoryData test = *(firstFreeAddress);
	
		printf("%d\n", test.size);
		printf("%d\n", test.isFree); 
		
		printf("%s\n", "HOME STRETCH!!!");
		
		printf("%d\n", (char*)firstFreeAddress + sizeof(MemoryData));
		
		return (char*)firstFreeAddress + sizeof(MemoryData);
	} else {
		printf("There is not enough space in memory in order to allocated the amount requested in File: '%s' Line: '%d'\n", myfile, line);
		return NULL;
	}				 
}

void myfree(void * mementry, char * myfile, int line) {
	
	printf("%d\n", mementry - sizeof(MemoryData));

	printf("Nimrod\n");
	
	// We start the pointer at mainMemory, which is the start of the char array.
	
	MemoryData* ptr = (MemoryData *)memoryblock;
	printf("Does this even work\n");
	
	if (ptr == NULL) {
		printf("FUCK\n");
	}
	
	// Goes through the linked list of memory blocks until it reaches one whose address matches the address of the freed variable
	while (ptr != NULL) {
		printf("%s\n", "In Loop");
		printf("%d\n", ptr);

		if (mementry - sizeof(MemoryData) == ptr) {
			printf("Found memory block\n");
			/* 
			This code will also merge adjacent free memory blocks, so it checks to see if the next memory block is NULL or not.
			We do not need to iterate through a while loop because this check will take place after every free, ensuring that every
			single adjacent free memory block will be merged, preventing future adjacent free memory blocks.
			*/
			printf("%d\n", ptr->isFree);

			if (ptr->next != NULL) {
				/*
				If the next memory block is free, then set the size of the current memory block to its own size plus the size of the
				adjacent memory block, which will be its data size plus its metadata size. Also set the current memory block's next
				pointer to the next pointer of the merged memory block.
				*/
				if (ptr->next->isFree == TRUE) {
					ptr->size = ptr->size + ptr->next->size + sizeof(MemoryData *);
					ptr->next = ptr->next->next;
					printf("Merged blocks forwards\n");
				}					
			}
			if (ptr->prev != NULL) {
				/*
				If the previous memory block is free, then we need to remove the current memory block and merge it with the previous one.
				Because metadata comes before data in our code, we move all of the current memory block's metadata to the previous memory block.
				*/
				if (ptr->prev->isFree == TRUE) {
					ptr->prev->size = ptr->size + ptr->prev->size + sizeof(MemoryData *);
					ptr = ptr->prev;
					printf("Merged blocks backwards\n");
				}
			}
			// After checking to make sure all adjacent memory blocks are merged, set the block's isFree to TRUE.
			ptr->isFree = TRUE;
			printf("Freed\n");
			return;
		}
		
		if(ptr->next == NULL) {
			printf("%s\n", "It's null");
		}
	
		// Iterate through the linked list of memory blocks.
		ptr = ptr->next;
	}
	// If there is no memory block with a matching address, then no such variable was ever malloc'ed.
	printf("No such variable has been allocated in File: '%s' Line: '%d'\n", myfile, line);
	return;
}
