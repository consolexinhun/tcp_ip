#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 30;

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;
    pid_t pid;

    if(argc != 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error!");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        error_handling("connnect() error");
    }
    else {
        puts("connect");
    }

    pid = fork();
    if (pid == 0) {  // 子进程写
        while(1) {
            fputs("input message(Q to quit):", stdout);
            fgets(message, BUF_SIZE, stdin);

            if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
                /* 向服务器端传递 EOF ，由于 fork 复制了文件描述符，所以 1 次 close 不够 */
                shutdown(sock, SHUT_WR);  
                break;
            }

            write(sock, message, strlen(message));
        }
    } else {  // 父进程读
        while(1) {
            str_len = read(sock, message, BUF_SIZE-1);
            if (str_len == 0) {
                break;
            }

            message[str_len] = 0;
            printf("message from server: %s", message);
        }
    }
    close(sock);

    return 0;
}