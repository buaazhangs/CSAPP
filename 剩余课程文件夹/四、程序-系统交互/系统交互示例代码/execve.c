#include<syscall.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#define MAXARGS 128
/*
关于 extern char **environ
extern 关键字：
extern 关键字用于声明一个变量是在其他地方定义的。
在这种情况下，它告诉编译器 environ 变量在程序的其他地方（通常是操作系统的运行时环境）已经被定义。

链接到外部：
当您在程序中使用 extern char **environ;，
编译器理解这是一个引用到程序外部定义的全局变量。这意味着 environ 不是由您的程序定义的，而是由操作系统在程序启动时提供的。

使用 environ：
通过声明 extern char **environ;，您的程序可以直接访问这个由操作系统维护的环境变量数组。
这是一种标准的方法，用于在 C 程序中访问环境变量。
*/
extern char **environ;
int main(){
    char *myargv[4] = {"/bin/ls","-lt","/usr/include",NULL};
    char **env = environ;
    pid_t pid;
    
    //打印环境变量
    // while(*env){
    //     printf("%s\n",*env);
    //     env++;
    // }
    // printf("\n\n\n");
    // scanf("%s",myargv[1]);

    if((pid = fork()) == 0){
        if(execve(myargv[0],myargv,environ) < 0){
            printf("%s:command not found\n",myargv[0]);
            exit(0);
        }
    }
    exit(0);
}