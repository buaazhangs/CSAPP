#include<csapp.h>

//sig类型，为一个返回类型void的函数指针，且参数为int
typedef void (*sighandler_t) (int);

void sigfunc(int sig){
    if(sig == SIGINT)
        printf("\"sigint\" signal is received\n");
    if(sig == SIGKILL)
        printf("\"SIGKILL\" signal is received\n");
    if(sig == SIGTERM)
        printf("\"SIGTERM\" signal is received\n");
    
    exit(0);
}

int main(){
    if(signal(SIGINT,sigfunc) == SIG_ERR){
        printf("signal error");
        exit(0);
    }
    //下面的执行必然是sigerr，不能为sigkill设置处理函数
    // if(signal(SIGKILL,sigfunc) == SIG_ERR){
    //     printf("signal error");
    //     exit(0);
    // }
    if(signal(SIGTERM,sigfunc) == SIG_ERR){
        printf("signal error");
        exit(0);
    }
    pause();
    printf("continue\n");
    exit(0);
}