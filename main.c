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

// handle signal and calc the count
int signalCount = 0;
void signalHandler(int signal)
{
	printf("Caught signal %d!\n", signal);
	signalCount++;
	printf("Current signal count: %d\n", signalCount);
}

int main(int argc, char *argv[])
{
	printf("*************************************************************************\n");

	// check customized command
	int random = -1;
	int a = -1;
	for (int q = 0; q < argc; q++)
	{
		if (strcasecmp(argv[q], "-l") == 0)
			int l = atoi(argv[q + 1]);
		if (strcasecmp(argv[q], "-u") == 0)
			int u = atoi(argv[q + 1]);
		if (strcasecmp(argv[q], "-e") == 0)
            random = -1
		if (strcasecmp(argv[q], "-r") == 0)
			random = 1;
		if (strcasecmp(argv[q], "-n") == 0)
			int childNum = atoi(argv[q + 1]);
	}

	// define the SIGUSR1 and SIGUSR2 so when the root node receive these signal, they know where to go
	signal(SIGUSR1, signalHandler);
	signal(SIGUSR2, signalHandler);

	// send all the info to the coord node to handle
	Root(getpid(), l, u, random, childNum);

	return 0;
}