#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <setjmp.h>
#include <execinfo.h>
#include <unistd.h>
#include <sys/syscall.h>

static int sigsegv_caught;
static pthread_t tid1;
static pthread_t tid2;
static sigjmp_buf env1;
static sigjmp_buf env2;

static void* thread_a(void *);
static void* thread_b(void *);
static void signal_handler(int , siginfo_t *, void *);

int main(int argc, char *argv[])
{
    sigsegv_caught = 0;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV,  &act, NULL);

    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  
    
    pthread_create( &tid1, 0, &thread_a, NULL );
    pthread_create( &tid2, 0, &thread_b, NULL );
    pthread_join(tid1, 0);
    pthread_join(tid2, 0);

    printf("main exit\n");
    return 0;
}

static void* thread_a(void *args)
{
    int ret1, ret2;
    char *cp = NULL;
    ret1 = setjmp(env1);

    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  

    if(ret1 == 5){
        ret2 = sigsetjmp(env2, 0);
        if(ret2 != 0){
            printf("thread A exit\n");
            pthread_exit(0);
        }
        printf("jump to thread A successful\n");
        *cp = '\0';
        pthread_exit(0);
    }
    pthread_exit(0);
}

static void* thread_b(void *args)
{
    sleep(5);   
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  
    
    longjmp(env1, 5);
    printf("thread B exit\n");
    pthread_exit(0);
}

static void* thread_c(void *args)
{
    sleep(5);   
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  
    while(1){
        sleep(3);
    }
    printf("thread C exit\n");
    pthread_exit(0);
}

static void signal_handler(int signal, siginfo_t *info, void *c)
{
    sigsegv_caught++;
    int i = 0;

    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  

    printf("signal is %d, SIGSEGV is %d\n", signal, SIGSEGV);  
    if(signal == SIGSEGV){
        
        siglongjmp(env2, 1);
        printf("thread SIG exit\n");
        return ;
    }
}
