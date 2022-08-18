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

int accept_sock, receive_sock;

void urg_handler(int sig);

void print_error() {
    printf("error is : %s\n", strerror(errno));
    exit(1);
}

int main(int argc, char **argv) {
    char buf[BUF_SIZE];

    struct sockaddr_in serv_addr, receive_addr;
    socklen_t serv_addr_size;
    int str_len, state;
    struct sigaction act;

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    accept_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(accept_sock == -1) {
        print_error();
    }

    memset(&receive_addr, 0, sizeof(receive_addr));
    receive_addr.sin_family = AF_INET;
    receive_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    receive_addr.sin_port = htons(atoi(argv[1]));

    if(bind(accept_sock, (struct sockaddr *)&receive_addr, sizeof(receive_addr)) == -1) {
        print_error();
    }

    if(listen(accept_sock, 5) == -1) {
        print_error();
    }

    serv_addr_size = sizeof(serv_addr);

    receive_sock = accept(accept_sock, (struct sockaddr*)&serv_addr, &serv_addr);
    if(receive_sock == -1) {
        print_error();
    }
    else {
        printf("connect client %d\n", receive_sock);
    }

    fcntl(receive_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while((str_len = recv(receive_sock, buf, BUF_SIZE-1, 0)) != 0) {
        if (str_len == -1) {
            continue;
        }
        buf[str_len] = 0;
        puts(buf);
    }
    close(receive_sock);
    close(accept_sock);

    return 0;
}