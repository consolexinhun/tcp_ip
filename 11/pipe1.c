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

#define BUF 30
int main(int argc, char** argv) {
    int fds[2];  // 0 表示管道出口，1 表示管道入口
    char str[] = "who are you ? ";
    char buf[BUF];
    pipe(fds);
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        write(fds[1], str, sizeof(str));
    } else {
        read(fds[0], buf, BUF);
        puts(buf);
    }
    return 0;
}
