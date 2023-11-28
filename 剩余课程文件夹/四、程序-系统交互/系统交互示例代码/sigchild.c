#include<csapp.h>

ssize_t sio_puts(char s[]){
    return write(STDOUT_FILENO,s,strlen(s));
}

#define n 5
int cnt = n;

//这个只处理一个子进程，cnt只减一次
void handler1(int sig){
    int olderrno = errno;
    //回收子进程的终止后状态信息（PID，退出状态等）
    if((waitpid(-1,NULL,0)) < 0){
        write(STDOUT_FILENO,"waitpid error",strlen("waitpid error"));
    }
    cnt--;
    sio_puts("父进程收到一个子进程信号\n");
    sleep(1);
    errno = olderrno;
}

//这个收到信号就处理所有终止的子进程，每处理一个cnt--
void handler2(int sig){
    int olderrno = errno;
    __pid_t pid;
    //回收子进程的终止后状态信息（PID，退出状态等）
    //waitpid返回值为终止子进程的pid，如果没有返回-1，
    //因此该waitpid加循环的使用让父进程回收所有子进程的终止状态，即使只有一个sigchil信号
    sio_puts("父进程收到一个子进程信号\n");
    while((pid = waitpid(-1,NULL,0)) > 0){
        char buffer[128];
        sprintf(buffer,"回收了子进程%d\n",(int)pid);
        write(STDOUT_FILENO,buffer,strlen(buffer));
        cnt--;
    } 
    sleep(1);
    errno = olderrno;
}

//sigchil信号处理程序
void sigchil1(){
    if(signal(SIGCHLD,handler2) == SIG_ERR){
        printf("signal error");
        exit(0);
    }

    for(int i = 0;i<n;i++){
        if(!fork()){
            write(STDOUT_FILENO,"子进程已被创建并执行\n",strlen("子进程已被创建并执行\n"));
            sleep(1);
            exit(0);
        }
    }

    while(cnt>0)
        ;
    exit(0);
}

//利用阻塞的sigchil信号处理程序
void sigchil2(){
    if(signal(SIGCHLD,handler1) == SIG_ERR){
        printf("signal error");
        exit(0);
    }

    for(int i = 0;i<n;i++){
        if(!fork()){
            write(STDOUT_FILENO,"子进程已被创建并执行\n",strlen("子进程已被创建并执行\n"));
            sleep(1);
            exit(0);
        }
    }

    while(cnt>0)
        ;
    exit(0);
}

int main(){
    sigchil1();
}