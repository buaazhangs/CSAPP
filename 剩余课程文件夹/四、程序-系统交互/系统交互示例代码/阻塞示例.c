#define _POSIX_C_SOURCE 200809L
#include<csapp.h>
#include<signal.h>
#include<unistd.h>
#include<x86_64-linux-gnu/bits/sigaction.h>
volatile long cnt = 2;
size_t t = 20;

void handler1(int sig){
    sigset_t mask,prev_mask;//初始都为空

    sigfillset(&mask);//这一步mask包含所有信号了
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);
    char buffer[10];
    sprintf(buffer,"%ld\n",--cnt);
    write(STDOUT_FILENO,buffer,strlen(buffer));
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);
    _exit(0);
}

int main(){
    __pid_t pid;
    sigset_t mask,prev_mask;
    printf("%ld\n",cnt);
    fflush(stdout);
    signal(SIGUSR1,handler1);
    if((pid = fork()) == 0){
        while(1){
            ;
        }
    }
    kill(pid,SIGUSR1);
    waitpid(-1,NULL,0);

    sigfillset(&mask);//这一步mask包含所有信号了
    sigprocmask(SIG_BLOCK,&mask,&prev_mask);
    char buffer[10];
    sprintf(buffer,"%ld\n",++cnt);
    write(STDOUT_FILENO,buffer,strlen(buffer));
    sigprocmask(SIG_SETMASK,&prev_mask,NULL);
    exit(0);
}
