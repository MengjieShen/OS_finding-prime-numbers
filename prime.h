

void swap(int *a, int *b);
void randomize ( int arr[], int n );
void delegatorParent(int fdBack[]);
void delegator(pid_t ppid, int childNum, int delegatorArr[], int delegatorArrLength, int random);
void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum, char* fifoArr);
void worker(pid_t pppid, int start, int end, int delegatorArr[], int fd[]);
int prime(int n);