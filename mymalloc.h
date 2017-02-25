#include <stdlib.h>
#include <stdio.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define	free(x) myfree(x, __FILE__, __LINE__)
#define memorySize 5000
#define FALSE 0
#define TRUE 1
#define boolean char

typedef struct _MemoryData {
	struct _MemoryData * next;
	struct _MemoryData * prev; 
	short int size;
	boolean isFree;  
}MemoryData; 
