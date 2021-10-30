#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "prime.h"


int signalCount1 = 0;
int signalCount2 = 0;

void signalHandler(int signo)
{
    switch(signo) {
    case SIGUSR1: //handle SIGUSR1
        printf("Root : catch SIGUSR1\n");
        signalCount1++;
        break;
    case SIGUSR2: //handle SIGUSR2
        printf("Root : catch SIGUSR2\n");
        signalCount2++;
        break;
    default:      
        printf("Should not be here\n");
        break;
    }
}

int main(int argc, char *argv[])
{
	printf("*************************************************************************\n");

	// check customized command
	int random = -1;
	int l,u,childNum;

	for (int q = 0; q < argc; q++)
	{
		if (strcmp(argv[q], "-l") == 0){
			l = atoi(argv[q + 1]);
        }
		if (strcmp(argv[q], "-u") == 0){
 			u = atoi(argv[q + 1]);           
        }
		if (strcmp(argv[q], "-e") == 0){
            random = -1;
        }
		if (strcmp(argv[q], "-r") == 0){
			random = 1;
        }
		if (strcmp(argv[q], "-n") == 0){
			childNum = atoi(argv[q + 1]);
        }
	}
	
	// handle SIGUSR1 and SIGUSR2 
    if(signal(SIGUSR1, signalHandler) == SIG_ERR)
    {
        perror("Can't set handler for SIGUSR1\n");
        exit(1);
    }

    if(signal(SIGUSR2, signalHandler) == SIG_ERR)
    { 
        perror("Can't set handler for SIGUSR2\n");
        exit(1);
    }

	// send all the info to the root node to handle
	root(getpid(), l, u, random, childNum);
    // root(getpid(), 1, 100, -1, 2);
	printf("total signal 1 caught: %d \n", signalCount1);
    printf("total signal 2 caught: %d \n", signalCount2);
	return 0;
}
