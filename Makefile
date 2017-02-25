PROGRAMS = mymalloc memgrind
CC = gcc

mymallocMake: mymalloc.o memgrind.o
	$(CC) -o memgrind mymalloc.o memgrind.o
	 
.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)