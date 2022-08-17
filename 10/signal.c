#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("timeout");
    }
    alarm(2);  // 可以注释这个试试
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        puts("ctrl + c pressed");
    }
}

int main(int argc, char** argv) {
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);

    alarm(2);

    for (i = 0; i < 3; i++) {
        puts("wait");
        sleep(10);
    }
    return 0;
}
