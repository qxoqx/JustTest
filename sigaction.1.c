#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>

static pthread_t tid_a;
static pthread_t tid_ft;

void sig_handler(int signum, siginfo_t *info,void *c)
{
    printf("getpid = %u, siginfo_pid = %u\n", getpid(), info->si_pid);
    printf("signal thread id = 0x%lx\n", pthread_self());
    if(info->si_pid == 0){
        if (pthread_equal(pthread_self(), tid_ft)){
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

    // sigset_t mask;
    // sigemptyset(&mask);
    // sigaddset(&mask, SIGSEGV);
    // pthread_sigmask(SIG_SETMASK, &mask, NULL);

    printf("child thread a thread id = 0x%lx, pid = %u\n", pthread_self(), getpid());
    sleep(20);
    // pthread_kill(tid_ft, SIGSEGV);
    //  *cp = '\0';
    printf("child thread a exit\n");
    pthread_exit(0);
}

static void* thread_ft(void *args)
{
    char *cp = NULL; 

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGSEGV);
    pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
    // pthread_sigmask(SIG_SETMASK, &mask, NULL);
    
    printf("child thread ft thread id = 0x%lx, pid = %u\n", pthread_self(), getpid());
    sleep(5);

    // if (s != 0)
    //     handle_error_en(s, "sigwait");
    //printf("Signal handling thread got signal %d\n", sig);

    //raise(SIGSEGV);
    //  pthread_kill(tid_ft, SIGSEGV);
    //printf("send kill\n");
    //kill(getpid(), SIGSEGV);
    //kill(syscall(SYS_gettid), SIGSEGV);
    sigqueue(syscall(SYS_gettid), SIGSEGV, (const union sigval)NULL);
    sleep(30);
    //*cp = '\0';
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

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGSEGV);
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    printf("child thread m thread id = 0x%lx, pid = %u\n", pthread_self(), getpid());

    pthread_create( &tid_a, 0, &thread_a, NULL );
    pthread_create( &tid_ft, 0, &thread_ft, NULL );

    printf("child thread m tid_a = 0x%lx, tid_ft = 0x%lx\n", tid_a, tid_ft);
    pthread_join(tid_a, 0);
    pthread_join(tid_ft, 0);

    while(1){
        sleep(3);
    }
    return 0;
}

