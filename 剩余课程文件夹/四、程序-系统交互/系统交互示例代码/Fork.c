#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

//错误信息
void unix_Error(char *msg){
    fprintf(stderr,"%s\n",msg);
    exit(0);
}
//fork包装
pid_t Fork(void){
    pid_t pid = fork();
    if (pid < 0)
        unix_Error("fork error");
    return pid;
}

int main(){
    __pid_t pid;
    int x = 1;
    pid = Fork();
    if (pid == 0){
        printf("child process:%d\n",++x);
    }
    printf("father or child:%d\n",--x);
    exit(0);
}