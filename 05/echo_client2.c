#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 1024;

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;

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

    while(1) {
        fputs("input message(Q to quit):", stdout);

        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while(recv_len < str_len) {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE-1);
            if(recv_cnt == -1) {
                error_handling("read() error");
            }
            recv_len += recv_cnt;
        }

        // str_len = read(sock, message, BUF_SIZE-1);
        // message[str_len] = 0;
        message[recv_len] = 0;
        printf("message from server: %s", message);
    }
    close(sock);

    return 0;
}