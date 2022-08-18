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
    char str1[] = "who are you ? ";
    char str2[] = "thank you for message";
    char buf[BUF];
    pipe(fds);
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        read(fds[0], buf, BUF);
        printf("child process output: %s\n", buf);
    } else {
        read(fds[0], buf, BUF);
        printf("parent process output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
