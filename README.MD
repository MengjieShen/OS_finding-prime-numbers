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

```./primes -l LowerBound -u UpperBound -[e|r] -n NumOfNodes```

## reference 
shuffle algorithm:
https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/

non blocking pipe:
https://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/