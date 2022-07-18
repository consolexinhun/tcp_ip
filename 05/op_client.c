#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 1024;
const int RLT_SIZE = sizeof(int);
const int OPSIZE = sizeof(int);

int main (int argc, char **argv) {
    int sock;
    char opmsg[BUF_SIZE];

    int result, op_cnt, i;

    struct sockaddr_in addr;
    if(argc != 3) {
        printf("Usage : %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        error_handling("connect() error!");
    }
    else {
        puts("connect!");
    }

    fputs("operator count : ", stdout);
    scanf("%d", &op_cnt);
    opmsg[0] = (char)op_cnt;

    for(i = 0; i < op_cnt; i++) {
        printf("oper %d:", i+1);
        scanf("%d", (int*)&opmsg[i*OPSIZE+1]);
    }

    fgetc(stdin);

    fputs("operator:", stdout);
    scanf("%c", &opmsg[op_cnt * OPSIZE + 1]);
    write(sock, opmsg, op_cnt*OPSIZE + 2);
    read(sock, &result, RLT_SIZE);

    printf("Operation result: %d\n", result);
    close(sock);
    return 0;
}