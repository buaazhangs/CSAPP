#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

#include<sys/wait.h>
#include<errno.h>

//不能回收的僵尸进程
void fork7(){
    pid_t pid;
    if((pid = fork()) == 0){
        printf("child process's father pid:%d\n",getppid());
        printf("child process pid:%d\n",getpid());
    }
    else{
        printf("father process pid:%d\n",getpid());
        while(1);
    }
    exit(0);
}

//父进程终结，子进程仍在运行
void fork8(){
    if(fork() == 0){
        printf("child process pid:%d\n",getpid());
        while(1);
    }
    else{
        printf("father process pid:%d\n",getpid());
        exit(0);
    }
}
//wait函数控制线程
void fork9(){
    int child_status;
    if(fork()){
        printf("child processer\n");
        exit(0);
    }
    else{
        wait(&child_status);
        printf("child processer has terminated\n");
        printf("father processer\n");
    }
    exit(0);
}
//运行时增加&符号能够后台运行
int main(){
    fork9();
}