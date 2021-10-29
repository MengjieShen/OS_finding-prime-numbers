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
#define MAX_CHILDREN 3

// void childhandler(int fds[]){
//     int tmp = 3;
//     if (write(fds[1], &tmp, sizeof(int)) <= 0) //write from pipe
//     {
//         perror( "write failed" );
//         exit(1);
//     }

//     printf("write child = %d\n", tmp);
// }

// int main( void )
// {
//     pid_t pid;

//     int fd[MAX_CHILDREN-1][2];

//     for (int j = 0; j < MAX_CHILDREN; j++){
//         if(pipe(fd[j]) == -1)
//         {
//             perror( "pipe Failed" );
//         }
//         if(fcntl( fd[j][0], F_SETFL, fcntl(fd[j][0], F_GETFL) | O_NONBLOCK)<0){
//             perror("fail to be non-blocking");
//         }
//     }     

//     for(int num_process = 0; num_process < MAX_CHILDREN; num_process++)
//     {

        
//         pid = fork();

//         if(pid < 0)
//         {
//             perror("fork failed");
//             exit(1);
//         }

//         if(pid == 0)
//         { //child code
//             for (int i = 0; i < MAX_CHILDREN; i++){
//                 if (i != num_process){
//                     close(fd[i][0]);
//                     close(fd[i][1]);
//                 }
//             }
//             printf("num_process: %d\n", num_process);
//             close(fd[num_process][0]);
//             printf("Child %i (pid= %i)\n", num_process, getpid());
//             childhandler(fd[num_process]);
//             close(fd[num_process][1]);
//             exit(0);
//         }

//         else{//parent
//             close(fd[num_process][1]);
//             while(1){
//                 int buff;
//                 printf("Im parent %i of %d\n",getpid(), num_process);
//                 int nread = read( fd[num_process][0], &buff, sizeof(int));
//                 if (nread > 0){
//                     printf("Parent receive %d\n", buff);
//                     sleep(1);
//                 }else if (nread == 0)
//                 {
//                     printf("end\n");
//                     break;
//                 }
                
//                 else{
//                     printf("read nothing\n");
//                     sleep(1);
//                 };

//             }
//             close(fd[num_process][0]);
//             // wait(NULL);         
//         }
//     }

//     return 0;
// }

int* generateArr(int start, int end, int Arr[]){
    int workerArrLength = end - start;
    // int workerArr[workerArrLength];
    int* workerArr = calloc(workerArrLength, sizeof(int));
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
            workerArr[workerArrIndex] = Arr[track];
            workerArrIndex++;
            // printf("check worker array 85 %d\n", workerArr[workerArrIndex]);
        }
        track++;   
    }
    return workerArr;
}

int main(){
    // int* workerArr = calloc(workerArrLength, sizeof(int));
    // int arr[] = {1,2,3,4,5,6,7,8,9,10};
    // int *worker;
    // worker = generateArr(3, 6, arr);
    // int tmp[10];
    // for (int i = 0; i < 3; i++){
    //     // printf("%d\n", worker[i]);
    //     tmp[i] = worker[i];
    // }
    // for (int i = 0; i < 3; i++){
    //     printf("%d\n", tmp[i]);
    //     // tmp[i] = worker[i];
    // }
    // int randomnumber=rand(n[]);
    int* result;
    result = (int*)malloc(5 * sizeof(int));

    result[0] = 1;
    for(int i = 0; i < 3; i++){
        printf("%d \n", result[i]);
    }
}