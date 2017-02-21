#include <stdlib.h>
#include <stdio.h>

#define malloc(x) mymalloc(x, _FILE_, _LINE_)
#define	malloc(x) myfree(x, _FILE_, _LINE_)

