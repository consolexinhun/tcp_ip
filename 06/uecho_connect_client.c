#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 1024;

int main(int argc, char** argv) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;
    // struct sockaddr_in serv_addr, client_addr;
    // socklen_t addr_size;

    if(argc != 3) {
        printf("Usage: %s <IP> <Port\n>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while(1) {
        fputs("insert message(q to quit):", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        // sendto(sock, message, strlen(message), 0, 
        //     (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        write(sock, message, strlen(message));

        // addr_size = sizeof(client_addr);
        // str_len = recvfrom(sock, message, BUF_SIZE, 0, 
        //     (struct sockaddr*)&client_addr, &addr_size);

        str_len = read(sock, message, sizeof(message)-1);
        message[str_len] = 0;
        printf("message from server: %s", message);
    }

    return 0;
}