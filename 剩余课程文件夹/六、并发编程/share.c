#include "csapp.h"
#include <pthread.h>
void *thread(void *var);
char **ptr;

int main(){
    pthread_t tid;
    char* msgs[2] = {"hello from foo","hello from bar"};
    ptr = msgs;
    for(int i = 0;i < 2;i++){
        //(void*)强转表示把i直接变成一个指向任意类型指针的类型，其数据不变
        //不是说创建了一个指针指向i
        pthread_create(&tid,NULL,thread,(void *) i);
    }
    pthread_exit(NULL);
}

void *thread(void *var){
    int myid = (int)var;
    static int cnt = 0;
    printf("[%d]: %s (cnt = %d)\n",myid,ptr[myid],++cnt);
    return NULL;
}