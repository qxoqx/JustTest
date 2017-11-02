#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
    struct stat buf;
    stat("test", &buf);
    printf("%s%s%s\n", ctime(&buf.st_atime), ctime(&buf.st_mtime), ctime(&buf.st_ctime));
    return 0;
}