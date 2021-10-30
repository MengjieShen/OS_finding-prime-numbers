# Finding Prime numbers using processes and pipes

This is a program termed primes that computes all the prime numbers found in an interval of integers [a, b]. The task is accomplished with the help of processes enmeshed in a hierarchy. The latter is created using fork() calls. The processes participating in the hierarchy help collaboratively carry out the computation in question.

In general, the concurrent operations of all processes in the hierarchy follow a divide-and-conquer style of action: a number of nodes (processes) are expected to work on their own and identify primes in sub-intervals of [a, b]. Other processes will operate as delegators of work and help asynchronously collect produced results as well as contribute in compiling a sorted outcome.

## How to compile and run my program
### From the Command Line
``` make ```
``` ./main -l LowerBound -u UpperBound -[e|r] -n NumOfNodes ```

-l indicates the minimun integer or LowerBound of the interval

-u indicates the maximum integer or UpperBound of the interval

-e indicates that work is to be distributed equally among all delegators and workers. If the flag 

-r is used instead, sub-intervals are arranged in a random manner for both delegators and workers

-n provides the fixed number of offspring processes every node has to create at the top–2 levels of the process hierarchy.

one example is that:

```./main -l 1 -u 200000 -e -n 2```

## Files:
### main.c: 
handle the flags and start the whole program.

### structure.c: 
the main structure of the process hierarchy, including functions for root node, delegator node, worker node.
	
### helper.c: 
include those functions that help the main structure to run properly. For example, randomize algorithm, sorting algorithms and finding primes algorithms. 

### prime.h: 
I defined two different node struct for one linked list storing result of prime numbers and the other linked list storing result of the execution time. I choose to use the linked list so that I can dynamically allocate my memory. 

## reference 
shuffle algorithm:
https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/

non blocking pipe:
https://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/