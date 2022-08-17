#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>

const int BUFFER = 30;

int main(int argc, char** argv) {
    int serv_sock, client_sock;
    FILE* fp;
    char buf[BUFFER];
    int read_cnt;

    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_size;

    if (argc != 2) {
        printf("usage: %s port\n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c", "rb");
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 5);

    client_size = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_size);

    while(1) {
        read_cnt = fread((void*)buf, 1, BUFFER, fp);
        if (read_cnt < BUFFER) {
            write(client_sock, buf, read_cnt);
            break;
        }
        write(client_sock, buf, BUFFER);
    }
    shutdown(client_sock, SHUT_WR);
    read(client_sock, buf, BUFFER);
    printf("message from client: %s\n", buf);

    fclose(fp);
    close(client_sock);
    close(serv_sock);

    return 0;
}
