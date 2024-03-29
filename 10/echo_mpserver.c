#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <signal.h>

#include "error_handle.h"

const int BUF_SIZE = 30;

void read_childproc(int sig);

int main(int argc, char** argv) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;

    pid_t pid;

    struct sigaction act;
    socklen_t addr_size;
    int str_len, state;

    char buf[BUF_SIZE];
    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    act.sa_handler = read_childproc;  // 处理子进程，防止僵尸进程
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    while(1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &addr_size);
        if(client_sock == -1) {  // 有一个连接失败不能影响其他的连接
            continue;
        }
        else {
            printf("connect client \n");
        }

        pid = fork();
        if(pid == -1) {
            close(client_sock);
            continue;
        }
        else if(pid == 0) {  // 子进程
            close(serv_sock);  // 由于服务器套接字复制给了子进程，所以子进程要销毁
            while((str_len = read(client_sock, buf, BUF_SIZE)) != 0) {
                write(client_sock, buf, str_len);
            }
            close(client_sock);
            puts("client disconnect!");
            return 0;
        }
        else {  // 父进程
            close(client_sock);  // 因为客户端套接字已经复制给了子进程，所以服务器要销毁自己的
        }

    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id:%d\n", pid);
}