#include "mymalloc.h"

int main(int argc, char ** argv)  {
	
	int * x, y, z, w, v, u;
	x = (int *)malloc(1); 
	y = (int *)malloc(10);
	z = (int *)malloc(100); 
	v = (int *)malloc(1000);
	u = (int *)malloc(10000);
	myfree(x);
	myfree(z);
	myfree(x);
	myfree(y);
	myfree(v);
	myfree(v);
	return 0; 
}