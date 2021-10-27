
# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation
CC = gcc
CFLAGS = -pedantic -Wall -g
LIBS = -lm

# ****************************************************
# Entries to bring the executable up to date

main: main.o structure.o
	$(CC) $(CFLAGS) -o main main.o structure.o $(LIBS)
	
main.o: main.c prime.h
	$(CC) $(CFLAGS) -c main.c

structure.o: structure.c prime.h
	$(CC) $(CFLAGS) -c structure.c

clean: 
	rm -f *~ *.o a.out core main

