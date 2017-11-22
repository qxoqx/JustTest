#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argv, char *argc[])
{
    pid_t tid;
    tid = strtol(argc[1], NULL, 10);
    printf("%u will kill %u\n", getpid(), tid);
    //sigqueue(tid, SIGSEGV, (const union sigval)NULL);
    kill(tid, SIGSEGV);
    return 0;
}
