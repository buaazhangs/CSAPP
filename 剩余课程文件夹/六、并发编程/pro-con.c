#include "csapp.h"
#include "sbuf.h"
#include "pthread.h"
#include <stdlib.h>

#define buffersize (int)100
//gcc pro-con.c /root/workspace/CSAPP/code/code/conc/sbuf.c -o pro-con 
//-I /root/workspace/CSAPP/code/code/conc -lcsapp -lpthread

int ans[buffersize] = {0};

void* producer1(void *sbuffer);
void* producer2(void *sbuffer);
void* consumer(void *sbuffer);

int main(){
    sbuf_t *buffer = malloc(sizeof(sbuf_t));
    
    sbuf_init(buffer,buffersize);
    unsigned long tid1,tid2,tid3;
    pthread_create(&tid1,NULL,producer2,buffer);
    pthread_create(&tid3,NULL,producer1,buffer);
    pthread_create(&tid2,NULL,consumer,buffer);

    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    for(int i=0;i<buffersize;i++){
        printf("%d,",ans[i]);
    }
    printf("\n");

    exit(0);
}

void* producer1(void *sbuffer){
    for(int i = 1;i <= buffersize/2;i++){
        sbuf_insert((sbuf_t *)sbuffer,i*2-1);
    }
    return NULL;
}

void* producer2(void *sbuffer){
    for(int i = 1;i <= buffersize/2;i++){
        sbuf_insert((sbuf_t *)sbuffer,i*2);
    }
    return NULL;
}

void* consumer(void *sbuffer){
    for(int i = 0;i < buffersize;i++){
        ans[i] = sbuf_remove((sbuf_t *)sbuffer);
    }
    return NULL;
}
