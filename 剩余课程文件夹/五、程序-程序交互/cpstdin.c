#include "csapp.h"
#include <stdlib.h>

int main(){
    char buf[100];
    char std = '\n';
    while(read(STDIN_FILENO,&buf,1)){
        write(STDOUT_FILENO,&buf,1);
        write(STDOUT_FILENO,&std,1);
    }
    exit(0);
}