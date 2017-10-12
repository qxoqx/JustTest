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
static pthread_t tid3;
static sigjmp_buf env1;
static sigjmp_buf env2;

static void* thread_a(void *);
static void* thread_b(void *);
static void* thread_c(void *);
static void signal_handler(int , siginfo_t *, void *);

int main(int argc, char *argv[])
{
    sigsegv_caught = 0;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV,  &act, NULL);

<<<<<<< HEAD
    printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
    printf("%s child thread tid = %u\n", __func__, pthread_self());  
=======
    //printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
    printf("%s child thread tid = %u\n", __func__, pthread_self());   
>>>>>>> origin/master
    
    pthread_create( &tid1, 0, &thread_a, NULL );
    pthread_create( &tid2, 0, &thread_b, NULL );
    pthread_create( &tid3, 0, &thread_c, NULL );
    pthread_join(tid1, 0);
    pthread_join(tid2, 0);
    pthread_join(tid3, 0);

    printf("main exit\n");
    return 0;
}

static void* thread_a(void *args)
{
    int ret1, ret2;
    char *cp = NULL;
    ret1 = setjmp(env1);
<<<<<<< HEAD

    printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
=======
    //printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
>>>>>>> origin/master
    printf("%s child thread tid = %u\n", __func__, pthread_self());   

    if(ret1 == 5){
        ret2 = sigsetjmp(env2, 0);
        if(ret2 != 0){
            printf("sig_hangler jump to thread A successful\n");
            //pthread_exit(0);
            while(1){                
                printf("Restart now\n");
                sleep(5);
            }
            //return;
        }
<<<<<<< HEAD
        printf("B jump to A successful\n");
=======
        printf("B jump to thread A successful\n");
>>>>>>> origin/master
        *cp = '\0';
        pthread_exit(0);
    }
    pthread_exit(0);
}

static void* thread_b(void *args)
<<<<<<< HEAD
{
    sleep(5);   
    printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
    printf("%s child thread tid = %u\n", __func__, pthread_self());  
    
=======
{ 
    //printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
    printf("%s child thread tid = %u\n", __func__, pthread_self());  
 
    sleep(5);  
>>>>>>> origin/master
    longjmp(env1, 5);
    printf("thread B exit\n");
    pthread_exit(0);
}

static void* thread_c(void *args)
{
    //printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
    printf("%s child thread tid = %u\n", __func__, pthread_self());  
 
    while(1){
        sleep(3);
        printf("This thread C\n");
        
    }
    pthread_exit(0);
}

static void signal_handler(int signal, siginfo_t *info, void *c)
{
    //sigsegv_caught++;
    int i = 0;

<<<<<<< HEAD
    printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
=======
    printf("signal is %d, SIGSEGV is %d\n", signal, SIGSEGV);  
    //printf("%s child thread lwpid = %u\n", __func__, syscall(SYS_gettid));  
>>>>>>> origin/master
    printf("%s child thread tid = %u\n", __func__, pthread_self());  

    if(signal == SIGSEGV){
        if(tid2 == pthread_self()){
            //restart thread
<<<<<<< HEAD
            siglongjmp(env2, 1);
        }else{
            //exit()
            exit(-1);
        }
        printf("signal is %d, SIGSEGV is %d\n", signal, SIGSEGV);  
=======
            printf("right tid\n");
            siglongjmp(env2, 1);
        }else{
            //exit(-1)
            printf("eixt\n");
            exit(-1);
        }
        //siglongjmp(env2, 1);
        //printf("thread SIG exit\n");
        return ;
    }else{
        printf("other signal\n");
>>>>>>> origin/master
    }
    exit(0);
}
