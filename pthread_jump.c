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

static void *getMcontextEip(ucontext_t *uc) {  
    /* Linux */  
#if defined(__i386__)  
return (void*) uc->uc_mcontext.gregs[14]; /* Linux 32 */  
#elif defined(__X86_64__) || defined(__x86_64__)  
return (void*) uc->uc_mcontext.gregs[16]; /* Linux 64 */  
#elif defined(__ia64__) /* Linux IA64 */  
return (void*) uc->uc_mcontext.sc_ip;  
#endif  
} 

extern void rcaladr();
//extern void rcaladr2();
long stack[16] = {0};

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

    printf("main:%p thread_a:%p thread_b:%p signal_handler:%p\n", main, thread_a, thread_b, signal_handler);

    printf("main exit\n");
    return 0;
}

static void* thread_a(void *args)
{
    int ret1, ret2;
    char *cp = NULL;
    ret1 = setjmp(env1);
    rcaladr(4, &stack[0]);
    //printf("stacka[0]:%p\nstacka[1]:%p\nstacka[2]:%p\n", (void *)stack[0], (void *)stack[1], (void *)stack[2]);
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  
    
    printf("%s: %p\n", __func__, __builtin_return_address(0));
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
    // while(1){
    //     printf("thread A sleeping\n");
    //     sleep(2);
    // }
    pthread_exit(0);
}

static void* thread_b(void *args)
{
    sleep(5);
    rcaladr(4, &stack[0]);    
    //printf("stackb[0]:%p\nstackb[1]:%p\nstackb[2]:%p\n", (void *)stack[0], (void *)stack[1], (void *)stack[2]);
    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  
    
    printf("%s: %p\n", __func__, __builtin_return_address(0));
    longjmp(env1, 5);
    // while(1){
    //     printf("thread B sleeping\n");
    //     sleep(1);
    // }
    //sleep(5);
    printf("thread B exit\n");
    pthread_exit(0);
}

static void signal_handler(int signal, siginfo_t *info, void *c)
{
    ucontext_t *uc = (ucontext_t*) c;
    sigsegv_caught++;
    void *buffer[30] = {0};
    size_t size = 0;
    char **strings = NULL;
    int i = 0;
    // printf("%s: %p\n", __func__, __builtin_return_address(0));
    // printf("%s: %p\n", __func__, __builtin_return_address(1));
    // printf("%s: %p\n", __func__, __builtin_return_address(2));
    rcaladr(3, &stack[0]);
    //printf("stack[0]:%p\nstack[1]:%p\nstack[2]:%p\n", (void *)stack[0], (void *)stack[1], (void *)stack[2]);

    printf("child thread lwpid = %u\n", syscall(SYS_gettid));  
    printf("child thread tid = %u\n", pthread_self());  

    printf("signal is %d, SIGSEGV is %d\n", signal, SIGSEGV);  
    if(signal == SIGSEGV){
        size = backtrace(buffer, 30);
        if (getMcontextEip(uc) != NULL)  
            buffer[1] = getMcontextEip(uc);
        strings = backtrace_symbols(buffer, size);
        if (strings == NULL) {  
            perror("backtrace_symbols");  
            exit(EXIT_FAILURE);  
        }  

        for(i = 0; i < size; i++){
            printf("bt[%d]: %s\n", i, strings[i]);
            //printf("bt[%d]: %x\n", i, (int)buffer[i]);
        }
        free(strings);
        siglongjmp(env2, 1);
        printf("thread SIG exit\n");
        return ;
    }
}
