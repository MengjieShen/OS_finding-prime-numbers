
# Makefile for executable adjust

# *****************************************************
# Parameters to control Makefile operation
CC = gcc
CFLAGS = -pedantic -Wall -g
LIBS = -lm

# ****************************************************
# Entries to bring the executable up to date

main: main.o structure.o helper.o
	$(CC) $(CFLAGS) -o main main.o structure.o helper.o $(LIBS)
	
main.o: main.c prime.h
	$(CC) $(CFLAGS) -c main.c

structure.o: structure.c prime.h
	$(CC) $(CFLAGS) -c structure.c

helper.o: helper.c prime.h
	$(CC) $(CFLAGS) -c helper.c

clean: 
	rm -f *~ *.o structure test a.out core main

