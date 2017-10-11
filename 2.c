#include <stdio.h>
#include <unistd.h>
int main(void)
{
    FILE *fp;
    char outstring[20];
    unsigned int i = 0;
    while(1){
        //返回值 文件顺利打开后，指向该流的文件指针就会被返回。若果文件打开失败则返回NULL，并把错误代码存在errno 中。
        fp = fopen("/home/filetest","a+");
        if(fp == NULL){
            printf("file open failed\n");
            return 1;
        }
        sprintf(outstring, "TEST : write %u \n", i++);
        //fputs 返回值 若成功则返回写出的字符个数，返回EOF则表示有错误发生。
        if(fputs(outstring, fp) == EOF){
            printf("write failed");
            return 1;
        }
        fclose(fp);
        usleep(500000);
    }
    return 0;
}


#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
    FILE *fp;
    char cur_task_name[50];//大小随意，能装下要识别的命令行文本即可
    char buf[BUF_SIZE];

    fp = fopen("/home/status", "r");//打开文件
    if(fp == NULL){
        printf("fopen error!\n");
    }
    if( fgets(buf, BUF_SIZE-1, fp) == NULL ){
        fclose(fp);
        printf("fgets error!\n");
    }
    sscanf(buf, "%*s %s", cur_task_name);
    if(strcmp(cur_task_name, argv[1]) == 0){
        printf("right\n");
    }
    return 0;
}