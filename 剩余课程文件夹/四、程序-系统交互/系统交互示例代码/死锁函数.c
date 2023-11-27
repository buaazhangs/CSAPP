#include<csapp.h>

//sig类型，为一个返回类型void的函数指针，且参数为int
typedef void (*sighandler_t) (int);

void sigfunc(int sig){
    if(sig == SIGINT)
        printf("\"SIGINT\" signal is received\n");
    exit(0);
}

int main(){
    int i = 0,max = 100000;
    while(i < max){
        if(signal(SIGINT,sigfunc) == SIG_ERR){
        printf("signal error");
        exit(0);
    }
        printf("1\t");
    }
    return 0;
}