#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handle.h"

const int BUF_SIZE = 30;

int main(int argc, char **argv) {
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t client_addr_size;

    struct sockaddr_in serv_addr, client_addr;
    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1) {
        error_handling("UDP socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serv_sock, (struct sockaddr*)& serv_addr, sizeof(serv_addr)) == -1 ) {
        error_handling("bind() error");
    }

    while(1) {
        client_addr_size = sizeof(client_addr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, 
            (struct sockaddr*)&client_addr, &client_addr_size);
        sendto(serv_sock, message, str_len, 0, 
            (struct sockaddr*)&client_addr, client_addr_size);
    }
    close(serv_sock);
    return 0;
}