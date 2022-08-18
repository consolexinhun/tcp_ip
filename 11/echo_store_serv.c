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

/* 多进程服务器，并且用管道保存消息的内容到文件中 */

void read_childproc(int sig);
int main(int argc, char** argv) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t addr_size;
    int str_len, state;
    char buf[BUF];

    if (argc != 2) {
        printf("usage : %s port\n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    assert(ret != -1);

    ret = listen(serv_sock, 5);
    assert(ret != -1);

    pipe(fds);
    pid = fork();
    if (pid == 0) {
        /* 子进程写文件 */
        FILE* fp = fopen("msg.txt", "wt");
        char msgbuf[BUF];
        int i, len;
        for (i = 0; i < 10; i++) {
            len = read(fds[0], msgbuf, BUF);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock == -1) {
            continue;
        } else {
            puts("new client connected!");
        }

        pid = fork();
        if (pid == 0) {
            close(serv_sock);
            while ((str_len = read(client_sock, buf, BUF)) != 0) {
                write(client_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(client_sock);
            puts("client disconnected!");
            return 0;
        } else {
            close(client_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove child process id : %d\n", pid);
}
