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
#include <errno.h>
#include <fcntl.h>

const int BUF_SIZE = 30;


void print_error() {
    printf("error is : %s\n", strerror(errno));
    exit(1);
}

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    int accept_sock, receive_sock;

    struct sockaddr_in accept_addr, receive_addr;
    socklen_t receive_addr_size;
    int str_len, state;

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    accept_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (accept_sock == -1) {
        perror("socket error");
        print_error();
    }

    memset(&accept_addr, 0, sizeof(accept_addr));
    accept_addr.sin_family = AF_INET;
    accept_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    accept_addr.sin_port = htons(atoi(argv[1]));

    if (bind(accept_sock, (struct sockaddr *)&accept_addr, sizeof(accept_addr)) == -1) {
        perror("bind error");
        print_error();
    }

    if (listen(accept_sock, 5) == -1) {
        perror("listen error");
        print_error();
    }

    receive_addr_size = sizeof(receive_addr);
    receive_sock = accept(accept_sock, (struct sockaddr*)&receive_addr, &receive_addr_size);
    if (receive_sock == -1) {
        perror("accept error");
        print_error();
    }
    else {
        printf("connect client : %d\n", receive_sock);
    }

    while (1) {
        str_len = recv(receive_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
        if (str_len > 0) {
            break;
        }
    }

    buf[str_len] = 0;
    printf("buffering %d bytes : %s \n", str_len, buf);
    str_len = recv(receive_sock, buf, sizeof(buf)-1, 0);
    buf[str_len] = 0;
    printf("read again: %s \n", buf);
    close(accept_sock);
    close(receive_sock);
    return 0;
}

