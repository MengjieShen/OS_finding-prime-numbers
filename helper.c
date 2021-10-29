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
#include "prime.h"

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapNode(struct resultNode *a, struct resultNode *b)
{
    int temp = a->data;
    a->data = b->data;
    b->data = temp;
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

void Sort(int arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
 
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
 
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

void bubbleSort(struct resultNode *start)
{
    int swapped;
    struct resultNode *ptr1;
    struct resultNode *lptr = NULL;
  
    /* Checking for empty list */
    if (start == NULL)
        return;
  
    do
    {
        swapped = 0;
        ptr1 = start;
  
        while (ptr1->next != lptr)
        {
            if (ptr1->data > ptr1->next->data)
            { 
                swapNode(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

void printList(struct resultNode *start)
{ 
    struct resultNode *temp = start;
    // printf("\n");
    while (temp!=NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
}

void insertAtTheBegin(struct resultNode **start_ref, int data)
{
    struct resultNode *ptr1 = (struct resultNode*)malloc(sizeof(struct resultNode));
    ptr1->data = data;
    ptr1->next = *start_ref;
    *start_ref = ptr1;
}

int prime(int n){ 
    int i;
    if (n==1) return(NO); 
    for (i=2 ; i<n ; i++)
        if ( n % i == 0) return(NO); 
    return(YES);
}

// [start, end)
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

int freell(struct resultNode *head)
{
    struct resultNode *current = NULL;
    current = head;
    while (current != NULL)
    {
        head = head->next;
        free(current);
        current = head;
    }
    return 0;
}
