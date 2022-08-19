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

int main(int argc, char **argv) {
    int send_sock;
    struct sockaddr_in broad_addr;
    FILE* fp;
    char buf[BUF];
    if (argc != 3) {
        printf("usage: %s group_ip port\n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_addr, 0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = htons(atoi(argv[2]));
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);

    int so_brd = 1;
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if ((fp = fopen("news.txt", "r")) == NULL) {
        perror("fopen() error");
    }

    while (fgets(buf, BUF, fp) != NULL) {
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr));
        sleep(0.5);
    }
    fclose(fp);
    close(send_sock);

    return 0;
}
