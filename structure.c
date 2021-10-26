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
#define BUF_SIZE 1024

// int signalCount = 0;
// void signalHandler(int signal)
// {
// 	printf("Caught signal %d!\n", signal);
// 	signalCount++;
// 	printf("Current signal count: %d\n", signalCount);
// }


void delegator(pid_t ppid, int childNum, int delegatorArr[], int random){
    //set up the pipe
    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "%s: failed to create a pipe\n");
        exit(EXIT_FAILURE);
    }

    // // create n name pipes 
    // char fifoArr[childNum][BUF_SIZE];
    // char buffer[BUF_SIZE];
    // for (int m = 0; m < childNum; m++)
    // {
    //     sprintf(buffer, "fifo_delegator%d", m);
    //     strcpy(fifoArr[m], buffer);
    //     if (mkfifo(fifoArr[m], 0777) == -1){
    //         if (errno != EEXIST){
    //             printf("Could not create fifo file");
    //             return;
    //         }
    //     };
    // }

    //number of elements each process will handle
    //这个万一不是整数怎么办
    int numPerProcess = sizeof(delegatorArr) / childNum;
    if (random == -1){

    }else{

    }
    // create n worker nodes
    for (int i = 0; i < childNum; i++){
        if(fork() == 0){
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
            int start = i * numPerProcess;
            int stop = start + numPerProcess;
            if((i+1)*numPerProcess < sizeof(delegatorArr)){
                worker(getpid(), delegatorArr[i*numPerProcess, (i+1)*numPerProcess], i, fd);
            }else{
                worker(getpid(), delegatorArr[i*numPerProcess, sizeof(delegatorArr)], i, fd);
            }
            exit(0);
        }
    }

    pid_t pid;
    int status = 0;
    //OUTSIDE children loop - handle PARENT code
    while ((pid = wait(&status)) != -1)
    {
        int *primes; 
        read(fd[0], &primes, sizeof(primes));

    }
    //close WRITE end
    close(fd[1]);
    for(int i=0;i<childNum;i++) {
        wait(NULL);
    }
    
}

void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum, char* fifoArr){
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

void worker(pid_t pppid, int workerArr[], int n, int fd[]){
    //set up the write in buffer
    int buf[BUF_SIZE];
    int index = 0;

    // set up the timer
    double t1, t2, cpu_time; 
    struct tms tb1, tb2; 
    double ticspersec;
    int i, sum = 0;
    ticspersec = (double) sysconf(_SC_CLK_TCK); 
    t1 = (double) times(&tb1);
    
    // set up the pipe
    close(fd[0]);

    // char fifoName[100];
    // sprintf(fifoName, "fifo%d", n);
    // int fd = open(fifoName, O_WRONLY);
    
    for (i=0 ; i <= sizeof(workerArr) ; i++) 
        if ( prime(i)==YES )
            buf[index] = i;
            index ++;
            // printf("%d ",i);
    // printf("\n");

    // write into the pipe
    if(write(fd[1], &buf, sizeof(buf)) == -1){
        printf("Can‘t write to the fifo");
    }
    close(fd);

    // send signal to the root node
    kill(pppid, SIGUSR1);

    // end the timer and print out the time used
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
    printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time) .\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);

}

int prime(int n){ 
    int i;
    if (n==1) return(NO); 
    for (i=2 ; i<n ; i++)
        if ( n % i == 0) return(NO); 
    return(YES);
}

// int main(){
//     root(getpid(), 3, 20, 1, 2);
//     return 0;
// }