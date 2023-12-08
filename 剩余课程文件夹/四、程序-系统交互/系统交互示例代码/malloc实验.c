#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main(){
    int *a;
    char *b;
    a = (int*)malloc(8 * sizeof(int));
    b = (char*)a;
    a[0] = 1;
    a[1] = 2;
    char buffer[50];
    sprintf(buffer,"%d\t%d\t%d\n",*a,*(a+1),*(a+2));
    write(STDOUT_FILENO,buffer,8);
    exit(0);
}