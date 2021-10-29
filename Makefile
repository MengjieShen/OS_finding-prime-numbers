
# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation
CC = gcc
CFLAGS = -pedantic -Wall -g
LIBS = -lm

# ****************************************************
# Entries to bring the executable up to date

main: main.o structure.o helper.o
	$(CC) $(CFLAGS) -o main main.o structure.o helper.o
	
main.o: main.c prime.h
	$(CC) $(CFLAGS) -c main.c

structure.o: structure.c prime.h
	$(CC) $(CFLAGS) -c structure.c

helper.o: helper.c prime.h
	$(CC) $(CFLAGS) -c helper.c

run: main
	./main -l 1 -u 10 -r -n 2
clean: 
	rm -f *~ *.o structure test a.out core main

