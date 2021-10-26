#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h>
#include <sys/times.h> /* times() */ 
#include <unistd.h> /* sysconf() */
#include <time.h>
#include "prime.h"

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



void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

// void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum, char* fifoArr){
//     for (int i = 0; i < childNum; i++){
//         if(fork() == 0){
//             printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid());
//             delegator(getpid(),childNum);
//             exit(0);
//         }
//     }
//     for(int i=0;i<childNum;i++) {
//         wait(NULL);
//     }

//     //sort the result

//     //handle signal

// }

void worker(pid_t pppid, int start, int end, int delegatorArr[], int fd[]){
    // set up the timer
    double t1, t2, cpu_time; 
    struct tms tb1, tb2; 
    double ticspersec;
    ticspersec = (double) sysconf(_SC_CLK_TCK); 
    t1 = (double) times(&tb1);

    int workerArrLength = end - start;
    int workerArr[workerArrLength];
    int track = 0;
    int workerArrIndex = 0;
    for (int j = 0; j<2; j++){
        workerArr[j] = 0; 
    }
    while(1){
        if (track >= start && track < end){
            workerArr[workerArrIndex] = delegatorArr[track];
            workerArrIndex++;
            // printf("check worker array 85 %d\n", workerArr[workerArrIndex]);
        }
        if (track == end){
            break;
        }
        track++;
        
    }

    
    
    //set up the write in buffer
    // int buf[BUF_SIZE];
    // int index = 0;

    // close the read end
    close(fd[0]);
    
    for (int i=0 ; i <= workerArrLength ; i++){ 
    
        // printf("check array %d %d\n",i,workerArr[i]);
        if ( prime(workerArr[i])==YES ){
            // buf[index] = i;
            // index ++;
            int tmp = workerArr[i];
            printf("check number %d\n", tmp);
         
            if(write(fd[1], &tmp, sizeof(int)) == -1){
                 printf("Can‘t write to the pipe\n");
            }else{
                printf("process %d wrote successfully!\n", getpid());
            }
        }
    }

    // write into the pipe


    // close the write end
    close(fd[1]);

    // send signal to the root node
    // kill(pppid, SIGUSR1);

    // end the timer and print out the time used
    t2 = (double) times(&tb2);
    cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
    printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time) .\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);
    sleep(1);
}

int prime(int n){ 
    int i;
    if (n==1) return(NO); 
    for (i=2 ; i<n ; i++)
        if ( n % i == 0) return(NO); 
    return(YES);
}

void delegatorParent(int fdBack[]){
    //close the write end
    close(fdBack[1]);

    while (1){
        // int primes[BUF_SIZE]; 
        int tmp;
        // int nread = read(fdBack[0], primes, BUF_SIZE);
        int nread = read(fdBack[0], &tmp, sizeof(tmp));

        switch (nread) {
        case -1:

            // case -1 means pipe is empty and errono
            // set EAGAIN
            if (errno == EAGAIN) {
                printf("(pipe empty)\n");
                sleep(5);
                break;
            }

            else {
                perror("read");
                exit(4);
            }
        case 0:
            printf("End of conversation\n");

            // read link
            close(fdBack[0]);
            exit(0);
        default:
            // text read
            // by default return no. of bytes
            // which read call read at that time
            // for (int j = 0; j < sizeof(primes); j++){
                printf("MSG = %d\n", tmp);}
            // }
            
            
    }
}


void delegator(pid_t ppid, int childNum, int delegatorArr[], int delegatorArrLength, int random)
{
    int fdBack[2], i;
 
    // error checking for pipe
    if (pipe(fdBack) < 0)
        exit(1);
 
    // error checking for fcntl
    if (fcntl(fdBack[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);

    //number of elements each process will handle
    //这个万一不是整数怎么办
    int numPerProcess = delegatorArrLength / childNum;

    //ramdomly shuffle the array to handle the random
    if (random != -1){
        randomize (delegatorArr, delegatorArrLength);
    }



    for (int childP = 0; childP < childNum; childP ++){

        switch (fork()) {
            case (pid_t) -1:
                fprintf(stderr, "Call to fork failed.\n"); 
                exit(1);
        
            case 0:
                printf("This is the child process%d\n", getpid());
                if((childP+1)*numPerProcess <= delegatorArrLength){

                    worker(getppid(), childP *numPerProcess, (childP+1)*numPerProcess, delegatorArr, fdBack);
                }else{
                    worker(getppid(), childP *numPerProcess, delegatorArrLength, delegatorArr, fdBack);
                }
                exit(0);
            default:
                delegatorParent(fdBack);
        }
    } 
}



int main(){
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int arrLength = sizeof(arr)/sizeof(arr[0]);
    delegator(getpid(), 4, arr, arrLength, -1);
    return 0;
}