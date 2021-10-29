#define YES 1 
#define NO 0

typedef struct resultNode{
    int data;
    struct resultNode *next;
}resultNode;

void swap(int *a, int *b);
void swapNode(struct resultNode *a, struct resultNode *b);
void insertAtTheBegin(struct resultNode **start_ref, int data);
void randomize ( int arr[], int n );
void printList(struct resultNode *start);
void delegatorParent(int fd[], int timePipe[], int rootPipe[]);
void rootParent(int fd[], resultNode** resultll);
void bubbleSort(struct resultNode *start);
void delegator(pid_t ppid, int childNum, int delegatorArr[], int delegatorArrLength, int rootPipe[], int random, int sigNum);
void root(pid_t pid, int lowerBound, int upperBound, int random, int childNum);
void worker(pid_t pppid, int start, int end, int delegatorArr[], int fd[], int timePipe[], int sigNum);
void Sort(int arr[], int n);
int prime(int n);
int* generateArr(int start, int end, int Arr[]);
void signalHandler(int signo);
int freell(struct resultNode *head);

