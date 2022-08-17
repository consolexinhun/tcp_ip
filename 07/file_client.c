#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>

const int BUF = 30;

int main(int argc, char** argv) {
    int sock;
    FILE* fp;

    char buf[BUF];
    int read_cnt;
    struct sockaddr_in addr;

    if (argc != 3) {
        printf("usage : %s ip port\n", argv[0]);
        exit(1);
    }
    fp = fopen("recive.cpp", "wb");
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    while ((read_cnt = read(sock, buf, BUF)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
    }
    puts("recive file data");
    write(sock, "thank you", 10);
    fclose(fp);
    close(sock);
    return 0;
}
