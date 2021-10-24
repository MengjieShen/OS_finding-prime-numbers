#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>
#include <sys/times.h> /* times() */ 
#include <unistd.h> /* sysconf() */
#define YES 1 
#define NO 0

// int signalCount = 0;
// void signalHandler(int signal)
// {
// 	printf("Caught signal %d!\n", signal);
// 	signalCount++;
// 	printf("Current signal count: %d\n", signalCount);
// }


void delegator(pid_t ppid, int childNum){

    // create n name pipes 
    char fifoArr[childNum][100];
    char buffer[100];
    for (int m = 0; m < childNum; m++)
    {
        sprintf(buffer, "fifo_delegator%d", m);
        strcpy(fifoArr[m], buffer);
        mkfifo(fifoArr[m], 0777);
    }

    // create n worker nodes
    for (int i = 0; i < childNum; i++){
        if(fork() == 0){
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
            exit(0);
        }
    }
    for(int i=0;i<childNum;i++) {
        wait(NULL);
    }
    
}
void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum){
    for (int i = 0; i < childNum; i++){
        if(fork() == 0){
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
            delegator(getpid(),childNum);
            exit(0);
        }
    }
    for(int i=0;i<childNum;i++) {
        wait(NULL);
    }

    //sort the result

    //handle signal

}

// void worker(pid_t pppid, int lowerBound, int upperBound){

//     // set up the timer
//     double t1, t2, cpu_time; 
//     struct tms tb1, tb2; 
//     double ticspersec;
//     int i, sum = 0;
//     ticspersec = (double) sysconf(_SC_CLK_TCK); 
//     t1 = (double) times(&tb1);


//     int lb=0, ub=0, i=0;
//     lb = lowerBound;
//     ub = upperBound;

//     if ( ( lb<1 ) || ( lb > ub ) ) { 
//         printf("usage: prime1 lb ub\n");
//         exit(1); }

//     for (i=lb ; i <= ub ; i++) 
//         if ( prime(i)==YES )
//             printf("%d ",i);
//     printf("\n");

//     // send signal to the root node
//     kill(pppid, SIGUSR1);


//     // end the timer and print out the time used
//     t2 = (double) times(&tb2);
//     cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
//     printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time) .\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);

// }

// int prime(int n){ 
//     int i;
//     if (n==1) return(NO); 
//     for (i=2 ; i<n ; i++)
//         if ( n % i == 0) return(NO); 
//     return(YES);
// }

int main(){
    root(getpid(), 3, 20, 1, 2);
    return 0;
}