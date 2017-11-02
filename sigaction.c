#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>
static pthread_t tid1;
static pthread_t tid2;

void sig_handler(int signum, siginfo_t *info,void *c){
    pid_t pid = 0;
    uid_t uid = 0;
    pid = getpid();
    uid = getuid();
    printf("getpid = %u, getuid = %u, siginfo_pid = %u, siginfo_uid = %u\n", pid, uid, info->si_pid, info->si_uid);
    printf("signal thread id = 0x%x, pid = %u, tid = %u\n", pthread_self(), getpid(), syscall(SYS_gettid));  
    pthread_exit(0);
    return;
}
//int systid;
static void* thread_a(void *args)
{
    char *cp = NULL;
    // sigset_t mask;
    // sigemptyset(&mask);  
    // sigaddset(&mask, SIGSEGV); 
    // pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
    
  //  printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread a thread id = 0x%x, pid = %u, tid = %u\n", pthread_self(), getpid(), syscall(SYS_gettid));  
 //   systid = syscall(SYS_gettid);
    sleep(5);
    *cp = '\0';
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
    
    // printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread b thread id = 0x%x, pid = %u, tid = %u\n", pthread_self(), getpid(), syscall(SYS_gettid));  
    sleep(1);
    //raise(SIGSEGV);
    //pthread_kill(tid2, SIGSEGV);
    //printf("send kill\n");
    //kill(getpid(), SIGSEGV);
    //kill(syscall(SYS_gettid), SIGSEGV);
    //sigqueue(syscall(SYS_gettid), SIGSEGV, (const union sigval)NULL);
    sleep(3);
    //*cp = '\0';
    printf("child thread b exit\n");
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

    //printf("thread_id = %u, SYS_gettid = %u\n", pthread_self(), syscall(SYS_gettid));
    printf("child thread m thread id = 0x%x, pid = %u, tid = %u\n", pthread_self(), getpid(), syscall(SYS_gettid));  
    
    pthread_create( &tid1, 0, &thread_a, NULL );
    pthread_create( &tid2, 0, &thread_ft, NULL );
    printf("child thread m tid1 = 0x%x, tid2 = 0x%x\n", tid1, tid2);
    pthread_join(tid1, 0);
    pthread_join(tid2, 0);

    sleep(10);
    // char *p = NULL;
    // *p = 123; 
    while(1){
        sleep(3);
    }
    return 0;
}

