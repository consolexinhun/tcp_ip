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

#define BUF 30

void print_error() {
    printf("error is : %s\n", strerror(errno));
    exit(1);
}

int main(int argc, char **argv) {
    int recv_sock;
    int str_len;
    char buf[BUF];
    struct sockaddr_in addr;
    struct ip_mreq join_addr;

    if (argc != 3) {
        printf("usage: %s group_ip port\n", argv[0]);
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind() error");
        print_error();
    }

    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr));
    while ((str_len = recvfrom(recv_sock, buf, BUF-1, 0, NULL, 0)) > 0) {
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);
    return 0;
}
