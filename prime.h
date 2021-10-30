#define YES 1 
#define NO 0

typedef struct resultNode{
    int data;
    struct resultNode *next;
}resultNode;

typedef struct timeNode{
    double data;
    struct timeNode *next;
}timeNode;

void swap(int *a, int *b);
void swapNode(struct resultNode *a, struct resultNode *b);
void insertAtTheBegin(struct resultNode **start_ref, int data);
void insertTimeAtTheBegin(struct timeNode **start_ref, double data);
void randomize ( int arr[], int n );
void printList(struct resultNode *start);
void reportTime(struct timeNode *start);
void delegatorParent(int fd[], int timePipe[], int rootPipe[], int rootTimePipe[]);
void rootParent(int fd[], resultNode** resultll, int timePipe[], timeNode** timell);
void bubbleSort(struct resultNode *start);
void delegator(pid_t ppid, int childNum, int delegatorArr[], int delegatorArrLength, int rootPipe[], int rootTimePipe[], int random, int sigNum);
void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum);
void worker(pid_t pppid, int start, int end, int delegatorArr[], int fd[], int sigNum);
void Sort(int arr[], int n);
int prime(int n);
int* generateArr(int start, int end, int Arr[]);
void signalHandler(int signo);
int freell(struct resultNode *head);
int freetimell(struct timeNode *head);

