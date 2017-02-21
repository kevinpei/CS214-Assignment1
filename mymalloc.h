#include <stdlib.h>
#include <stdio.h>

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define	free(x) myfree(x, __FILE__, __LINE__)

