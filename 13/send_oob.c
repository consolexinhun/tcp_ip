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
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;

    if(argc != 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        print_error();
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        print_error();
    }
    else {
        puts("connect");
    }

    write(sock, "123", sizeof("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);

    return 0;
}