#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
//#include <sys/syscall.h>
static pthread_t tid1;
static pthread_t tid2;

void sig_handler(int signum, siginfo_t *info,void *c){
    pid_t pid = 0;
    uid_t uid = 0;
    pid = getpid();
    uid = getuid();
    printf("getpid = %u, getuid = %u, siginfo_pid = %u, siginfo_uid = %u\n", pid, uid, info->si_pid, info->si_uid);
    printf("child thread tid = %u\n", pthread_self());  
    pthread_exit(0);
    return;
}

static void* thread_a(void *args)
{
    sigset_t mask;
    sigemptyset(&mask);  
    sigaddset(&mask, SIGSEGV); 
    pthread_sigmask(SIG_SETMASK, &mask, NULL);
    char *cp = NULL;
  //  printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  

    sleep(5);
    *cp = '\0';
    pthread_exit(0);
    
}

static void* thread_b(void *args)
{
    sigset_t mask;
    sigemptyset(&mask);  
    sigaddset(&mask, SIGSEGV); 
    pthread_sigmask(SIG_SETMASK, &mask, NULL);
    char *cp = NULL;
  //  printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  

    sleep(5);
    *cp = '\0';
    pthread_exit(0);
    
}


int main(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, NULL);

    //printf("thread_id = %u, SYS_gettid = %u\n", pthread_self(), syscall(SYS_gettid));
    printf("child thread tid = %u\n", pthread_self());  
    
    pthread_create( &tid1, 0, &thread_a, NULL );
    pthread_create( &tid2, 0, &thread_b, NULL );
    pthread_join(tid1, 0);
    pthread_join(tid2, 0);

    sleep(10);
    char *p = NULL;
    *p = 123; 
    while(1){
        sleep(3);
    }
    return 0;
}

