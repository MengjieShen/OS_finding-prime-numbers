#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/times.h>

#include "prime.h"


int signalCount1 = 0;
int signalCount2 = 0;

int main(int argc, char *argv[])
{
	printf("*************************************************************************\n");

	// check customized command
	int random = -1;
	int l,u,childNum;
	for (int q = 0; q < argc; q++)
	{
		if (strcasecmp(argv[q], "-l") == 0)
			l = atoi(argv[q + 1]);
		if (strcasecmp(argv[q], "-u") == 0)
			u = atoi(argv[q + 1]);
		if (strcasecmp(argv[q], "-e") == 0)
            random = -1;
		if (strcasecmp(argv[q], "-r") == 0)
			random = 1;
		if (strcasecmp(argv[q], "-n") == 0)
			childNum = atoi(argv[q + 1]);
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

	return 0;
}
