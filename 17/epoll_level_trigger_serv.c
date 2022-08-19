#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

const int BUF = 2;
const int EPOLL = 50;

int main(int argc, char **argv) {
    int server_sock, client_sock;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addr_size;
    int str_len, i;

    char buf[BUF];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    server_sock  = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bing() error");
        exit(1);
    }

    if (listen(server_sock, 5) == -1 ) {
        perror("listen() error");
        exit(1);
    }

    /* begin */
    epfd = epoll_create(EPOLL);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL);

    event.events = EPOLLIN;
    event.data.fd = server_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &event);
    /* end */


    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL, -1);
        if (event_cnt == -1) {
            perror("event_wait() error!");
            exit(-1);
        }

        puts("return epoll wait");
        for (i = 0; i < event_cnt; i++) {
            if (ep_events[i].data.fd == server_sock) {  // 如果是服务器套接字，那么需要将客户端套接字加入 epoll 例程
                addr_size = sizeof(client_addr);
                client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
                event.events = EPOLLIN;
                event.data.fd = client_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);

                printf("connect client : %d\n", client_sock);
            } else {  // 如果是客户端套接字，没收到消息需要从 epoll 例程中删除套接字
                str_len = read(ep_events[i].data.fd, buf, BUF);
                if (str_len == 0) {  // 没收到消息
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client : %d\n", ep_events[i].data.fd);
                } else {  // 收到消息了
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }

    }

    close(server_sock);
    close(epfd);
    return 0;
}
