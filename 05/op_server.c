#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 1024;
const int OPSIZE = sizeof(int);

int calculate(int opnum, int opnds[], char operator);

int main(int argc, char **argv) {
    int serv_sock, client_sock;
    char opinfo[BUF_SIZE];
    int result, op_cnt, i;
    int recv_cnt, recv_len;

    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error");
    }

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

    client_addr_size = sizeof(client_addr);
    for(i = 0; i < 5; i ++) {
        op_cnt = 0;
        client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        read(client_sock, &op_cnt, 1);

        recv_len = 0;
        while( (op_cnt * OPSIZE + 1) > recv_len) {
            recv_cnt = read(client_sock, &opinfo[recv_len], BUF_SIZE-1);
            recv_len += recv_cnt;
        }
        result = calculate(op_cnt, (int *)opinfo, opinfo[recv_len-1]);
        write(client_sock, (char*) &result, sizeof(result));
        close(client_sock);
    }

    return 0;
}


int calculate(int opnum, int opnds[], char op) {
    int result = opnds[0], i;
    switch(op) {
        case '+':
            for(i = 1; i < opnum; i++) result += opnds[i];
            break;
        case '-':
            for(i = 1; i < opnum; i++) result -= opnds[i];
            break;
        case '*':
            for(i = 1; i < opnum; i++) result *= opnds[i];
            break;
        case '/':
            for(i = 1; i < opnum; i++) result /= opnds[i];
            break;
    }
    return result;
}