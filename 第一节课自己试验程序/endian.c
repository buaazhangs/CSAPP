#include<bits/stdc++.h>
#include<vector>

int main(){
    typedef unsigned char* pointer;
    int a = 15123;
    char* exam = (char*)&a;//以有符号数表示
    pointer start = (pointer)&a;//以无符号数表示
    for(int i = 0;i < 4;i++){
        printf("%p\t0x%.2x\n",start+i,start[i]);
    }
    return 0;
}