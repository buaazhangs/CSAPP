#include<stdio.h>
#include<stdlib.h>

int useless(){return 0;}
int global = 0;
void put_address(void* p,char* s){
    long temp;
    printf("address at %p\t%s\n",p,s);
    return;
}
int main(){
    int local = 0;
    void *a = malloc(100);
    put_address((void*)&local,"栈上局部变量");
    put_address((void*)&global,"全局变量");
    put_address((void*)a,"堆上局部变量");
    put_address((void*)useless,"代码段");
    free(a);
    return 0;
}