#include <linux/kernel.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>  

pthread_t ntid1;
pthread_t ntid2;

void signal_tmp1(void)
{
    FILE            *fp;
    int             ret;
    long            tmpid;
    char            *tmpp;
    pid_t           pid;
    unsigned char rbuf[12];    


/*
    fp = fopen((const char *)"/var/local/run/pbxwatchd.pid", "r");

    if(fp == NULL)
       exit(0); 

    memset(rbuf, 0, 12);
    tmpp = fgets((char *)rbuf, 11, fp);
    fclose(fp);

    tmpid = strtol((const char *)rbuf, NULL, 10); 

    if(tmpid <= 0)
    exit(0);

    pid = (pid_t)tmpid;

    if( getppid() != pid )
    {
        kill(pid, SIGCHLD);
    }
*/
   printf("signal thread is %x\n", pthread_self());
//    while(1)
//    {
//        sleep(5);
//        printf("signal is sleep\n");
//    }

    printf("exit thread is %x\n", pthread_self());
    pthread_exit(0);
    //exit(0);
}

/* void signal_tmp2(void)
{
    printf("exit2 thread is %x\n", pthread_self());
    printf("PID is %x\n", getpid());
    printf("PPID is %x\n", getppid());
    exit(0);
} */

void thrfun1(void)
{
    //struct sigaction sa_sig;
    //char *p = NULL;
    //sa_sig.sa_sigaction = (void *)signal_tmp2 ;
    //sa_sig.sa_flags     = SA_RESTART | SA_SIGINFO ;

    //sigaction( SIGTERM, &sa_sig, NULL );
    //sigaction( SIGINT , &sa_sig, NULL );
    int s, sig;
    printf("sub1 thread is %x\n", pthread_self());

    sigset_t mask;
    sigemptyset(&mask);  
    sigaddset(&mask, SIGSEGV); 
    s = sigwait(&mask, &sig);
    if (s != 0)
        printf("sigwait error\n");
    printf("Signal handling thread got signal %d\n", sig);

    while(1)
    {
        printf("sub1 thread is OK\n");
        sleep(5);
	    //*p = 123;
    }
}

void thrfun2(void)
{
    char *p = NULL;
    printf("sub2 thread is %x\n", pthread_self());

    // sigset_t mask;
    // sigemptyset(&mask);  
    // sigaddset(&mask, SIGSEGV); 
    // pthread_sigmask(SIG_SETMASK, &mask, NULL);
    //sleep(1);
    *p = 123;
    while(1)
    {
        printf("sub2 thread is OK\n");
        sleep(5);
    }
}

void main(void)
{
    struct sigaction sa_sig;

    sa_sig.sa_sigaction = (void *)signal_tmp1 ;
    sa_sig.sa_flags     = SA_RESTART | SA_SIGINFO ;

    sigaction( SIGTERM, &sa_sig, NULL );
    sigaction( SIGINT , &sa_sig, NULL );
    sigaction( SIGSEGV, &sa_sig, NULL );
    printf("\n");
    printf("main thread is %x\n", pthread_self());

    sigset_t mask;
    sigemptyset(&mask);  
    sigaddset(&mask, SIGSEGV); 
    pthread_sigmask(SIG_SETMASK, &mask, NULL);

    pthread_create(&ntid1,NULL,(void *)thrfun1,NULL);
    pthread_create(&ntid2,NULL,(void *)thrfun2,NULL);

    while(1)
    {
        printf("main thread is OK\n");
        sleep(5);
    }
    printf("main thread is OVER\n");
    return;
}
