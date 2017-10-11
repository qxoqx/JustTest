#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 1024
void getPidByName(char* task_name)
{
    DIR *dir;
    struct dirent *ptr;
    FILE *fp;
    char filepath[50];//大小随意，能装下cmdline文件的路径即可
    char cur_task_name[50];//大小随意，能装下要识别的命令行文本即可
    char buf[BUF_SIZE];
    dir = opendir("/proc"); //打开路径
    if (NULL != dir)
    {
        while ((ptr = readdir(dir)) != NULL) //循环读取路径下的每一个文件/文件夹
        {
            //如果读取到的是"."或者".."则跳过，读取到的不是文件夹名字也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))             
　　　　　　　　　　　　continue;
            if (DT_DIR != ptr->d_type) 
        　　　　　 continue;

            sprintf(filepath, "/proc/%s/status", ptr->d_name);//生成要读取的文件的路径
            fp = fopen(filepath, "r");//打开文件
            if (NULL != fp)
            {
                if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
            　　　　fclose(fp);
            　　　　continue;
        　　　　 }
        　　　　sscanf(buf, "%*s %s", cur_task_name);

                //如果文件内容满足要求则打印路径的名字（即进程的PID）
                if (!strcmp(task_name, cur_task_name))
           　　　　 printf("PID:  %s\n", ptr->d_name);
                fclose(fp);
            }

        }
        closedir(dir);//关闭路径
    }
}
void getNameByPid(pid_t pid, char *task_name) {
    char proc_pid_path[BUF_SIZE];
    char buf[BUF_SIZE];
    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE* fp = fopen(proc_pid_path, "r");
    if(NULL != fp){
        if( fgets(buf, BUF_SIZE-1, fp)== NULL ){
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}
void main(int argc, char** argv)
{
    char task_name[50];
    pid_t pid = getpid();
    printf("pid of this process:%d\n", pid);
    getNameByPid(pid, task_name);

    /*
    strcpy(task_name, argv[0]+2);
    printf("task name is %s\n", task_name);
    getPidByName(task_name);
    */
    printf("task name is %s\n", task_name);
    getPidByName(task_name);
    sleep(15);
}




/usr/local/bin/qemu-system-x86_64 -drive format=raw,file=/mnt/sv_cf1_192_167_99_50.img \
-m 2048 -cpu kvm64 -smp 2 -boot c \
-enable-kvm \
-monitor stdio -k en-us -vnc :0 -rtc base=localtime,clock=rt \
-net nic,vlan=0,model=virtio -net tap,vlan=0,ifname=tap3 \
-net nic,vlan=1,model=virtio -net tap,vlan=1,ifname=tap4


p/x sharm_atch->sharm_id = 0xae
p/x sharm_atch->proc_status[0] = 0xae
p/x proc_info[0].syslog_on = 0xae
p/x bmon_start.sw = 0xae
p/x pid_sv = 0xae
p/x gos_index = 0xae


b main.c:143
p/x sharm_atch->bmon_start.sw = 0xae
p/x sharm_atch->bmon_start.timer = 0xae
p/x sharm_atch->signal_wait.sw = 0xae
p/x sharm_atch->signal_wait.timer = 0xae
p/x sharm_atch->sigema_wait.sw = 0xae
p/x sharm_atch->sigema_wait.timer = 0xae
p/x sharm_atch->pid_sv = 0xae
p/x sharm_atch->flg_sv = 0xae
p/x sharm_atch->sig_sv = 0xae
p/x sharm_atch->softchg_flg = 0xae
p/x sharm_atch->proc_info[0].end_cnter = 0xae
p/x sharm_atch->proc_info[0].nrlst_time = 0xae
p/x sharm_atch->proc_info[0].syslog_on = 0xae
p/x sharm_atch->proc_info[3].end_cnter = 0xae
p/x sharm_atch->proc_info[3].nrlst_time = 0xae
p/x sharm_atch->proc_info[3].syslog_on = 0xae
p/x sharm_atch->proc_info[5].end_cnter = 0xae
p/x sharm_atch->proc_info[5].nrlst_time = 0xae
p/x sharm_atch->proc_info[5].syslog_on = 0xae
p/x sharm_atch->proc_info[6].end_cnter = 0xae
p/x sharm_atch->proc_info[6].nrlst_time = 0xae
p/x sharm_atch->proc_info[6].syslog_on = 0xae
p/x *sharm_atch
p/x proc_info
p/x proc_endmax
p/x clrtime
p/x syslog_out
p/x read_result
p/x bmon_start
p/x signal_wait
p/x sigema_wait
p/x stk_sigchld
p/x svema_fd
p/x main_tid
p/x opelamp_tid
p/x dipswchk_tid
p/x watchdog_tid
p/x clvmon_tid
p/x procstart_tid
p/x healthck_tid
p/x pid_sv
p/x flg_sv
p/x sig_sv
p/x softchg_flg
p/x svcpld_fd
p/x hcmng_mem
p/x hcsock_fd
p/x gos_domain_name
p/x gos_index
p/x virtual_IP_address
p/x host_IP_address
p/x sv95_srvkind
p/x sv95_gwatch
p/x softdog_fd
SIG 2
r
p/x sharm_atch->bmon_start
p/x sharm_atch->bmon_start
p/x sharm_atch->signal_wait
p/x sharm_atch->sigema_wait
p/x sharm_atch->pid_sv
p/x sharm_atch->flg_sv
p/x sharm_atch->sig_sv
p/x sharm_atch->softchg_flg
p/x *sharm_atch
p/x proc_info
p/x proc_endmax
p/x clrtime
p/x syslog_out
p/x read_result
p/x bmon_start
p/x signal_wait
p/x sigema_wait
p/x stk_sigchld
p/x svema_fd
p/x main_tid
p/x opelamp_tid
p/x dipswchk_tid
p/x watchdog_tid
p/x clvmon_tid
p/x procstart_tid
p/x healthck_tid
p/x pid_sv
p/x flg_sv
p/x sig_sv
p/x softchg_flg
p/x svcpld_fd
p/x hcmng_mem
p/x hcsock_fd
p/x gos_domain_name
p/x gos_index
p/x virtual_IP_address
p/x host_IP_address
p/x sv95_srvkind
p/x sv95_gwatch
p/x softdog_fd



watch ((SIPUALIBSES *)0xdf845ac)->next

sipUaAddSubSession
topSubscribe

handle SIGUSR2 noprint
b sptmrecv.c:3405
n
p ses->finalCode
p cseq
p ses->CSeq
c
n
p ses->finalCode
p cseq
p ses->CSeq
c
n
p ses->finalCode
p cseq
p ses->CSeq
c
n
p ses->finalCode
p cseq
p ses->CSeq
n
c


p/x ses->finalCode
n
p/x ses->finalCode
n
n
n
n
n
n
n
n
c

b sptm_rcvNotify

handle SIGUSR2 noprint
p/x topSubscribe
watch ((SIPUALIBSES *)0xaf2053bc)->CSeq
watch ((SIPUALIBSES *)0xaf2053bc)->finalCode
watch ((SIPUALIBSES *)0xaf20925c)->CSeq
watch ((SIPUALIBSES *)0xaf20925c)->finalCode
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
dis
c


handle SIGUSR2 noprint
p/x topSubscribe
watch ((SIPUALIBSES *)0xaf2053bc)->CSeq
watch ((SIPUALIBSES *)0xaf2053bc)->finalCode
watch ((SIPUALIBSES *)0xaf20925c)->CSeq
watch ((SIPUALIBSES *)0xaf20925c)->finalCode
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x *((sip_t*)(((SIPUALIBSES *)0xaf20925c)->ntfy_r))
p/x *(((sip_t*)(((SIPUALIBSES *)0xaf20925c)->ntfy_r))->cseq)
p/x *((((sip_t*)(((SIPUALIBSES *)0xaf20925c)->ntfy_r))->cseq)->number)
x/4b ((((sip_t*)(((SIPUALIBSES *)0xaf20925c)->ntfy_r))->cseq)->number)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
c
dis
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
c
p/x *((SIPUALIBSES *)0xaf2053bc)
p/x *((SIPUALIBSES *)0xaf20925c)
p/x ((SIPUALIBSES *)0xaf20925c)->finalCode = 0xae
c
dis
c






