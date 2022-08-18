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

#define BUF 100
#define PRINTERROR printf("error is : %s\n", strerror(errno))
        
int main(int argc, char** argv) {
    int serv_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    struct timeval timeout;
    fd_set reads, temps;

    socklen_t addr_size;
    int fd_max, str_len, fd_num, i;
    char buf[BUF];
    if (argc != 2) {
        printf("usage:%s port\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        PRINTERROR;
        exit(1);
    }
    if (listen(serv_sock, 5) == -1) {
        PRINTERROR;
        exit(1);
    }

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        fd_num = select(fd_max+1, &temps, 0, 0, &timeout);
        if (fd_num == -1) {
            puts("select() error");
            break;
        }
        if (fd_num == 0) {
            puts("timeout");
            continue;
        }
        for (i = 0; i < fd_max+1; i++) {
            if (FD_ISSET(i, &temps)) {  // 查找发生变化的套接字文件描述符
                if (i == serv_sock) {  // 服务端套接字
                    addr_size = sizeof(client_addr);
                    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &addr_size);
                    FD_SET(client_sock, &reads);

                    if (client_sock > fd_max) {
                        fd_max = client_sock;
                    }
                    printf("connect client: %d\n", client_sock);
                } else {
                    str_len = read(i, buf, BUF);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client : %d\n", i);
                    } else {
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    return 0;
}

