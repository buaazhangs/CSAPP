#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include<errno.h>

#define BUF_SIZE 1024

int main() {
    char buf[BUF_SIZE];
    ssize_t numRead, numWritten;

    int inputFd = open("input.txt", O_RDONLY);
    if (inputFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int outputFd = open("output.txt", O_WRONLY | O_CREAT, 0666);
    if (outputFd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        char *outPtr = buf;
        ssize_t nWritten;

        while (numRead > 0) {
            nWritten = write(outputFd, outPtr, numRead);
            if (nWritten >= 0) {
                numRead -= nWritten;//考虑不足值的处理
                outPtr += nWritten;
            } else {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    //考虑信号中断情况下的情形
    if (numRead == -1 && errno != EINTR) {
    // 真正的读取错误
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(inputFd) == -1) {
        perror("close input");
    }

    if (close(outputFd) == -1) {
        perror("close output");
    }

    exit(EXIT_SUCCESS);
}