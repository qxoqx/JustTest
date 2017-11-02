#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
static pthread_t tid_a;
static pthread_t tid_ft;

void sig_handler(int signum, siginfo_t *info,void *c)
{
    printf("getpid = %u, siginfo_pid = %u\n", getpid(), info->si_pid);
    printf("signal thread id = 0x%x\n", pthread_self());
    if(info->si_pid == 0){
        if(pthread_self() == tid_ft){
            printf("ft restart\n");
        }else{
            printf("Not Segmentation fault of ft!\n");
        }
    }else{
        printf("Not Segmentation fault!\n");
    }
    pthread_exit(0);
    return;
}

static void* thread_a(void *args)
{
    char *cp = NULL;

    printf("child thread a thread id = 0x%x, pid = %u\n", pthread_self(), getpid());  
    sleep(5);
    *cp = '\0';
    printf("child thread a exit\n");
    pthread_exit(0);
}

static void* thread_ft(void *args)
{
    char *cp = NULL;
    
    printf("child thread ft thread id = 0x%x, pid = %u\n", pthread_self(), getpid());  
    sleep(3);
    *cp = '\0';
    printf("child thread ft exit\n");
    pthread_exit(0);
}


int main(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, NULL);

    printf("child thread m thread id = 0x%x, pid = %u\n", pthread_self(), getpid());  
    
    pthread_create( &tid_a, 0, &thread_a, NULL );
    pthread_create( &tid_ft, 0, &thread_ft, NULL );
    printf("child thread m tid_a = 0x%x, tid_ft = 0x%x\n", tid_a, tid_ft);
    pthread_join(tid_a, 0);
    pthread_join(tid_ft, 0);

    while(1){
        sleep(3);
    }
    return 0;
}

