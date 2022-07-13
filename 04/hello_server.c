#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "error_handle.h"

int main(int argc, char *argv[])
{
    int serv_sock;
    int client_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;

    char message[] = "hello world!\n";

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建服务端套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("bing() error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        error_handling("listen() error");
    }

    client_addr_size = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_sock == -1)
    {
        error_handling("accept() error");
    }

    write(client_sock, message, sizeof(message));
    close(client_sock);
    close(serv_sock);

    return 0;
}