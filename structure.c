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
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "prime.h"


void delegatorParent(int fd[], int timePipe[], int rootPipe[], int rootTimePipe[]){

    while (1){
        int tmp;
        int nread = read(fd[0], &tmp, sizeof(tmp));
        if (nread == -1){
            // case -1 means pipe is empty and errono
            // set EAGAIN
            if (errno == EAGAIN) {
                // printf("(pipe empty)\n");
                sleep(1);
            }
            else {
                perror("read");
                exit(4);
            }
        }
        else if (nread == 0){
            break;
        }
        else{
            if(write(rootPipe[1], &tmp, sizeof(int)) == -1){
                 perror("Write failed");
            }
        }
    }
    // char exe_time[100];
    double exe_time;
    int timeRead = read(timePipe[0], &exe_time, sizeof(double));
    if (timeRead < 0){
        perror("time pipe read error");
    }else{
        // printf("Time = %lf \n", exe_time);
    }

    if(write(rootTimePipe[1], &exe_time, sizeof(double)) == -1){
        perror("Time write failed");
    }
}

void rootParent(int fd[], resultNode** resultll, int timePipe[], timeNode** timell){
    while (1){
        int tmp;
        int nread = read(fd[0], &tmp, sizeof(tmp));
        if (nread == -1){
            // case -1 means pipe is empty and errono
            // set EAGAIN
            if (errno == EAGAIN) {
                // printf("(pipe empty)\n");
                sleep(1);
            }
            else {
                perror("read");
                exit(4);
            }
        }
        else if (nread == 0){
            // printf("End of conversation\n");
            // sleep(1);
            break;
        }
        else{
            insertAtTheBegin(resultll, tmp);
            // printList(*resultll);
            // printf("root MSG = %d\n", tmp);
        }
    }
    while (1){
        double exe_time;
        int timeRead = read(timePipe[0], &exe_time, sizeof(double));
        if (timeRead == -1){
            if (errno == EAGAIN) {
                // printf("(pipe empty)\n");
                sleep(1);
            }
            else {
                perror("read");
                exit(4);
            }
        }else if (timeRead == 0)
        {
            break;
        }else{
            printf("Time = %lf \n", exe_time);
            insertTimeAtTheBegin(timell, exe_time);
        }
    }

}

void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum){

    //set up the pipe for delegator to transform data back to the root
    int fd[childNum][2];
    int timePipe[childNum][2];
    for (int j = 0; j < childNum; j++){
        // error checking for pipe
        if(pipe(fd[j]) < 0)
        {
            perror( "prime number pipe Failed" );
        }

        if(pipe(timePipe[j]) < 0)
        {
            perror( "time pipe Failed" );
        }

        // error checking for fcntl
        if(fcntl( fd[j][0], F_SETFL, fcntl(fd[j][0], F_GETFL) | O_NONBLOCK)<0){
            perror("fail to set the prime number pipe to be non-blocking");
        }

        if(fcntl( timePipe[j][0], F_SETFL, fcntl(timePipe[j][0], F_GETFL) | O_NONBLOCK)<0){
            perror("fail to set the time pipe to be non-blocking");
        }
        
    }   
    
    // initialize the array 
    int initArrayLength = upperBound-lowerBound+1;
    int initArray[initArrayLength];
    int ind = 0;
    for(int i = lowerBound; i <= upperBound; i++){
        initArray[ind] = i;
        ind ++;
    }

    // allocate array for result
    struct resultNode *resultll = NULL;
    struct timeNode *timell = NULL;
    //number of elements each process will handle
    int numPerProcess = initArrayLength / childNum;

     //handle the flag of random
    int randomArr[childNum+1];
    randomArr[0] = 0;
    srand(time(0));
    if (random != -1){
        for (int m = 1; m<childNum+1; m++){
            if (initArrayLength != 0){
                int j = rand() % initArrayLength;
                randomArr[m] = j;
            }else{
                randomArr[m] = 0;
            }

        }
    randomArr[childNum] = initArrayLength;
    Sort(randomArr, childNum+1);
    }

    // create childNum of delegators
    for (int childP = 0; childP < childNum; childP ++){
        pid_t pid = fork();
            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
        
            if(pid == 0){
                //calculate for the circular fashion
                int sig_num;
                if(childP % 2 == 0){
                    sig_num = 1;
                }else{
                    sig_num = 2;
                }

                // printf("This is the delegator process %d\n", getpid());
                //close the read and write pipe of other children
                for (int i = 0; i < childNum; i++){
                    if (i != childP){
                        close(fd[i][0]);
                        close(fd[i][1]);
                        close(timePipe[i][0]);
                        close(timePipe[i][1]);
                        }
                }
                //close the read pipe 
                close(fd[childP][0]);
                close(timePipe[childP][0]);
                if (random == -1){
                    if (childP!= childNum-1){
                        int *delegatorArr = generateArr(childP *numPerProcess, (childP+1)*numPerProcess, initArray);
                        delegator(getppid(), childNum, delegatorArr, numPerProcess ,fd[childP], timePipe[childP], random, sig_num);
                    }else{
                        int *delegatorArr = generateArr( childP *numPerProcess, initArrayLength, initArray);
                        delegator(getppid(), childNum, delegatorArr, initArrayLength - childP*numPerProcess ,fd[childP], timePipe[childP], random, sig_num);
                    }
                }else{
                    // printf("check random arr: %d, %d \n", randomArr[childP], randomArr[childP+1]);
                    int *delegatorArr = generateArr( randomArr[childP], randomArr[childP+1], initArray);
                    delegator(getppid(), childNum, delegatorArr, randomArr[childP+1] - randomArr[childP] ,fd[childP], timePipe[childP], random, sig_num);
                }
                close(fd[childP][1]);
                close(timePipe[childP][1]);
                exit(0);
            }
            else{
                close(fd[childP][1]);
                close(timePipe[childP][1]);
                rootParent(fd[childP], &resultll, timePipe[childP], &timell);
                close(fd[childP][0]);
                close(timePipe[childP][0]);
            }
    }
    
    for (int childP = 0; childP < childNum; childP ++){
        wait(NULL);
    }
    // printList(resultll);
    //sort the result
    bubbleSort(resultll);

    // //show on tty
    // printList(resultll);
    printf("\n");
    reportTime(timell);
    printf("\n");

    // clean the memory
    freell(resultll);
    freetimell(timell);
}

void worker(pid_t pppid, int start, int end, int delegatorArr[], int fd[], int sig_num){

    int workerArrLength = end - start;
    int workerArr[workerArrLength];
    int track = 0;
    int workerArrIndex = 0;
    for (int j = 0; j<workerArrLength; j++){
        workerArr[j] = 0; 
    }
    while(1){
        if (track == end){
            break;
        }
        if (track >= start && track < end){
            workerArr[workerArrIndex] = delegatorArr[track];
            workerArrIndex++;
            // printf("check worker array 85 %d\n", workerArr[workerArrIndex]);
        }
        track++;   
    }
    
    for (int i=0 ; i < workerArrLength ; i++){ 
    
        if ( prime(workerArr[i])==YES ){
            int tmp = workerArr[i];
            // printf("check number %d\n", tmp);
         
            if(write(fd[1], &tmp, sizeof(int)) == -1){
                 perror("Write failed");
            }
        }
    }

    // send signal to the root node
    if (sig_num == 1){
        kill(pppid, SIGUSR1);
    }else{
        kill(pppid, SIGUSR2);
    }

    // else{
    //     printf("process %d wrote execution of time successfully into the pipe!\n", getpid());
    // }
    
}

void delegator(pid_t ppid, int childNum, int delegatorArr[], int delegatorArrLength, int rootPipe[], int rootTimePipe[], int random, int sigNum)
{
    int fd[childNum][2];
    int timePipe[childNum][2];

    for (int j = 0; j < childNum; j++){
        // error checking for pipe
        if(pipe(fd[j]) < 0)
        {
            perror( "prime number pipe Failed" );
        }

        if(pipe(timePipe[j]) < 0){
            perror( "time pipe Failed");
        }

        // error checking for fcntl
        if(fcntl( fd[j][0], F_SETFL, fcntl(fd[j][0], F_GETFL) | O_NONBLOCK)<0){
            perror("fail to set the prime number pipe to be non-blocking");
        }

        // if(fcntl( timePipe[j][0], F_SETFL, fcntl(timePipe[j][0], F_GETFL) | O_NONBLOCK)<0){
        //     perror("fail to set the execution pipe to be non-blocking");
        // }
        
    }   

    //number of elements each process will handle
    int numPerProcess = delegatorArrLength / childNum;

    //split the array randomly
    int randomArr[childNum+1];
    randomArr[0] = 0;
    srand(time(0));
    
    if (random != -1){
        for (int m = 1; m<childNum+1; m++){
            if (delegatorArrLength != 0){
                int j = rand() % delegatorArrLength;
                randomArr[m] = j;
            }else{
                randomArr[m] = 0;
            }

    }
    randomArr[childNum] = delegatorArrLength;
    Sort(randomArr, childNum+1);
    }

    for (int childP = 0; childP < childNum; childP ++){
        pid_t pid = fork();
            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
        
            if(pid == 0){
                // set up the timer
                double t1, t2; 
                struct tms tb1, tb2; 
                double ticspersec;
                ticspersec = (double) sysconf(_SC_CLK_TCK); 
                t1 = (double) times(&tb1);
                // printf("ticspersec: %lf", ticspersec);

                // printf("This is the worker process %d\n", getpid());

                //close the read and write pipe of other children
                for (int i = 0; i < childNum; i++){
                    if (i != childP){
                        close(fd[i][0]);
                        close(fd[i][1]);
                        close(timePipe[i][0]);
                        close(timePipe[i][1]);
                        }
                }
                //close the read pipe 
                close(fd[childP][0]);
                close(timePipe[childP][0]);
                if (random == -1){
                    if (childP!= childNum-1){
                        worker(ppid, childP *numPerProcess, (childP+1)*numPerProcess, delegatorArr, fd[childP],  sigNum);
                    }else{
                        worker(ppid, childP *numPerProcess, delegatorArrLength, delegatorArr, fd[childP], sigNum);
                    }
                }else{
                    // printf("check random arr: %d, %d \n", randomArr[childP], randomArr[childP+1]);
                    worker(ppid, randomArr[childP], randomArr[childP+1], delegatorArr, fd[childP], sigNum);
                }

                close(fd[childP][1]);

                // end the timer and print out the time used
                t2 = (double) times(&tb2);
                // cpu_time = (double) ((tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime));
                // printf("Run time was %lf sec (REAL time) although we used the CPU for %lf sec (CPU time) .\n", (t2 - t1) / ticspersec, cpu_time / ticspersec);
                double execution_time =  (t2 - t1) / ticspersec;
                // char writeIn[100];
                // sprintf(writeIn, "%.2f", execution_time);
                if(write(timePipe[childP][1], &execution_time, sizeof(double)) == -1){
                    perror("Write failed");
                }
                close(timePipe[childP][1]);
                exit(0);
            }
            
            else{
                close(fd[childP][1]);
                close(timePipe[childP][1]);
                delegatorParent(fd[childP], timePipe[childP], rootPipe, rootTimePipe);
                close(fd[childP][0]);
                close(timePipe[childP][0]);
            }
        }
    for (int childP = 0; childP < childNum; childP ++){
        wait(NULL);
    }

}

