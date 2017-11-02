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

static void* glb_si_ptr;
static void* glb_si_addr;
static void* glb_si_call_addr;

pthread_mutex_t mutex;
#define ADDRS_SIZE 4

static const int addresses_size = 64;

static void* worker_thread( void* arg );
static void* worker_thread2( void* arg );

static void signal_handler(int signal, siginfo_t *info, void *c)
{
    int i = 0;

    printf("signal is %d, SIGSEGV is %d\n", signal, SIGSEGV);
    printf("%s child thread tid = %u\n", __func__, pthread_self());

    if(signal == SIGSEGV){
        if(tid1 == pthread_self()){
            //restart thread
            printf("right tid\n");
            if(mutex.__data.__lock == 1)
            {
                printf("SIGSEGV unlock\n");
                pthread_mutex_unlock(&mutex);
            }
            pthread_exit(0);
        }else{
            printf("exit\n");
            exit(-1);
        }
        return ;
    }else{
        printf("other signal\n");
    }
    exit(0);
}

int main(int argc, char *argv[])
{

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
    pthread_create( &tid2, 0, &worker_thread2, (void *)NULL );

    sleep(2);
    pthread_create( &tid1, 0, &worker_thread, (void *)NULL );
    pthread_join( tid1, 0 );
    //pthread_create( &tid1, 0, &worker_thread, (void *)NULL );
    //pthread_join( tid1, 0 );

    while(1)
    {
        sleep(3);
    }

    return 0;
}

static void* worker_thread(void* args)
{
    int ret;
    int lock = 0;

    printf("%s child thread tid = %u\n", __func__, pthread_self());
    printf("worker_thread lock\n");
    pthread_mutex_lock(&mutex);

    char *cp = NULL;    /* get SEGV */
    //*cp = '\0';

    printf("worker_thread unlock\n");
    pthread_mutex_unlock(&mutex);

    return NULL;
}

static void* worker_thread2(void* args)
{

    printf("%s child thread tid = %u\n", __func__, pthread_self());
    while(1)
    {
        printf("worker_thread2 lock\n");
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
        printf("worker_thread2 unlock\n");
        sleep(3);
    }

    return NULL;
}

