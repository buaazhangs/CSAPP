#include<stdio.h>

#define MAX_length 1145141919810
#define Mhz 1000000
long data[MAX_length];//实验的读取值，

//size为读取data的字节大小，即读取多大的内存，
//考验时间同步性，较小的可能一次性全在一级缓存了，较大的需要多级缓存甚至到内存；

//stride为读取步长，单位为数组索引,考验空间同步性
int test(int size,int stride){
    int i,st2 = stride * 2,st3 = stride * 3,st4 = stride * 4;
    int limit = size / sizeof(double);
    long acc1,acc2,acc3,acc4;
    for(i = 0;i < limit;i += st4){
        acc1 += data[i];
        acc2 += data[i+stride];
        acc3 += data[i+st2];
        acc4 += data[i+st3];
    }
    return acc1+acc2+acc3+acc4;
}

double run(int size,int stride){
    test(size,stride);//冷启动,预热各级缓存
    double cycles = fcyc2(test,size,stride,0);
    return (size/stride) * Mhz / cycles;
}

int main(){
    int size = 1000,stride = 1;
    printf("%lf",run(size,stride));
}

