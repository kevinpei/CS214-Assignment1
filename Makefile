PROGRAMS = mymalloc test

CC = gcc
CFLAGS = -Wall

%: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)