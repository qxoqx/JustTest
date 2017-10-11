#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <setjmp.h>
#include <execinfo.h>

static int sigsegv_caught;
static pthread_t tid1;
static pthread_t tid2;
static sigjmp_buf env;

// static void* glb_si_ptr;
// static void* glb_si_addr;
// static void* glb_si_call_addr;

//#define ADDRS_SIZE 4

//static const int addresses_size = 64;
//static int got_addresses_size = 0;
// static void* return_addresses[ADDRS_SIZE];
// static void* return_extract_addresses[ADDRS_SIZE];
// static void* frame_addresses[ADDRS_SIZE];

static void* worker_thread( void* arg );

// static void signal_handler(int signal, siginfo_t *info, void *c)
// {
//     sigsegv_caught++;
//     void* addr;
//     glb_si_addr = info->si_addr;

//     //got_addresses_size = backtrace(addresses, addresses_size);
//     unsigned int i;
//     printf("%s(0): %p\n", __func__, __builtin_return_address(0));
//     printf("%s(1): %p\n", __func__, __builtin_return_address(1));
//     printf("%s(2): %p\n\n", __func__, __builtin_return_address(2));

//     printf("%s(0): %p\n", __func__, __builtin_extract_return_addr(__builtin_return_address(0)));
//     printf("%s(1): %p\n", __func__, __builtin_extract_return_addr(__builtin_return_address(1)));
//     printf("%s(2): %p\n\n", __func__, __builtin_extract_return_addr(__builtin_return_address(2)));

//     printf("%s(0): %p\n", __func__, __builtin_frame_address(0));
//     printf("%s(1): %p\n", __func__, __builtin_frame_address(1));
//     printf("%s(2): %p\n", __func__, __builtin_frame_address(2));
//     //printf("%s(3): %p\n", __func__, __builtin_return_address(3));
//     //printf("%s(4): %p\n", __func__, __builtin_return_address(4));
//     for(i = 0; i < ADDRS_SIZE; i++) {
//         addr = __builtin_return_address(0); //it needs const value
//         return_addresses[i] = addr;

//         addr = __builtin_extract_return_addr(addr);
//         return_extract_addresses[i] = addr;
        
//         addr = __builtin_frame_address(0); //it needs const value
//         frame_addresses[i] = addr;

//         printf("%s(0): return_addresses:%p, return_extract_addresses:%p, __builtin_frame_address:%p\n",
//              __func__, return_addresses[i], return_extract_addresses[i], frame_addresses[i]);
//     }

//     siglongjmp(env, 1);
// }

static void *getMcontextEip(ucontext_t *uc) {  
#if defined(__APPLE__) && !defined(MAC_OS_X_VERSION_10_6)  
    /* OSX < 10.6 */  
    #if defined(__x86_64__)  
    return (void*) uc->uc_mcontext->__ss.__rip;  
    #elif defined(__i386__)  
    return (void*) uc->uc_mcontext->__ss.__eip;  
    #else  
    return (void*) uc->uc_mcontext->__ss.__srr0;  
    #endif  
#elif defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)  
    /* OSX >= 10.6 */  
    #if defined(_STRUCT_X86_THREAD_STATE64) && !defined(__i386__)  
    return (void*) uc->uc_mcontext->__ss.__rip;  
    #else  
    return (void*) uc->uc_mcontext->__ss.__eip;  
    #endif  
#elif defined(__linux__)  
    /* Linux */  
    #if defined(__i386__)  
    return (void*) uc->uc_mcontext.gregs[14]; /* Linux 32 */  
    #elif defined(__X86_64__) || defined(__x86_64__)  
    return (void*) uc->uc_mcontext.gregs[16]; /* Linux 64 */  
    #elif defined(__ia64__) /* Linux IA64 */  
    return (void*) uc->uc_mcontext.sc_ip;  
    #endif  
#else  
    return NULL;  
#endif  
}

static void signal_handler(int signal, siginfo_t *info, void *c)
{
    ucontext_t *uc = (ucontext_t*) c;
    sigsegv_caught++;
    void *buffer[30] = {0};
    size_t size = 0;
    char **strings = NULL;
    int i = 0;
    printf("%s(0): %p\n", __func__, __builtin_return_address(0));
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
        }
        free(strings);
        siglongjmp(env, 1);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_t mutex;

    sigsegv_caught = 0;
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = signal_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV,  &act, NULL);

    if(pthread_mutex_init(&mutex, NULL) < 0){
        printf("mutex init error\n");
    }
    //pthread is called as qemu_thread in QEMU
    pthread_create( &tid1, 0, &worker_thread, &mutex );
    pthread_join( tid1, 0 );

    pthread_create( &tid2, 0, &worker_thread, &mutex );
    pthread_join( tid2, 0 );

    printf("sigsegv:%d\n", sigsegv_caught);
    /* printf("glb_si_ptr:%p glb_si_addr:%p glb_si_call_addr:%p\n",
            glb_si_ptr, glb_si_addr, glb_si_call_addr); */
    printf("main:%p signal_handler:%p worker_thread:%p\n",
            main, signal_handler, worker_thread);
    //printf("buitin addr:%p\n", tmpaddr);

    // int i;
    // for(i = 0; i < got_addresses_size; i++) {
    //     printf("stack frame[%d]:%p\n", i, return_addresses[i]);
    // }

    return 0;
}

static void* worker_thread(void* args)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)args;
    int ret;
    int lock = 0;
    ret = sigsetjmp(env, 0);
    if(ret != 0) {
        printf("jump sucessful\n");
        printf("jump sucessful\n");
        printf("jump sucessful\n");
        printf("jump sucessful\n");
        printf("jump sucessful\n");
        printf("jump sucessful\n");
        if(lock == 1)
        {
            pthread_mutex_unlock(mutex);
            lock = 0;
        }
        return NULL;
    }
    pthread_mutex_lock(mutex);
    lock = 1;

    printf("%s(0): %p\n", __func__, __builtin_return_address(0));
    char *cp = NULL;    /* get SEGV */
    *cp = '\0';

    lock = 0;
    pthread_mutex_unlock(mutex);
    

    return NULL;
}
//dmesg
//SIGSEGV 发生时的堆栈情况 位置定位 诊断

