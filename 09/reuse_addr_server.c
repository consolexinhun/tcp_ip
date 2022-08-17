#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>

int main(int argc, char** argv) {
    int server_sock, client_sock;
    char message[30];
    int option, str_len;
    socklen_t optlen, client_size;
    struct sockaddr_in server_addr, client_addr;

    if (argc != 2) {
        printf("usage : %s port\n", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(server_sock != -1);

    optlen = sizeof(option);
    option = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    assert(ret != -1);

    ret = listen(server_sock, 5);
    assert(ret != -1);

    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);

    while ((str_len = read(client_sock, message, sizeof(message))) != 0) {
        write(client_sock, message, str_len);
        write(1, message, str_len);
    }
    close(client_sock);
    close(server_sock);
    return 0;
}
