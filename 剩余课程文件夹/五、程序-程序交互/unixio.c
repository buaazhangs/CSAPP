//#include "csapp.h"
#include "csapp.h"
#include <stdlib.h>

int main(){
    int fd1,fd2;
    fd1 = open("markdown.md",O_RDONLY,0);
    printf("fd1 = %d\n",fd1);
    close(fd1);
    exit(0);
}