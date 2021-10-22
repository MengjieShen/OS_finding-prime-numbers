
int signalCount = 0;
void signalHandler(int signal)
{
	printf("Caught signal %d!\n", signal);
	signalCount++;
	printf("Current signal count: %d\n", signalCount);
}