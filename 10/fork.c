#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>

int gval = 10;

int main(int argc, char** argv) {
    pid_t pid;
    int lval = 20;
    gval++;  // 11
    lval += 5;  // 25

    pid = fork();
    if (pid == 0) {
        gval += 2;
        lval += 2;
    } else {
        gval -= 2;
        lval -= 2;
    }

    if (pid == 0) {
        printf("child process: [%d %d]\n", gval, lval);
    } else {
        printf("parent process: [%d %d]\n", gval, lval);
    }
    return 0;
}
