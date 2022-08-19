#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

const int BUF_SIZE = 1024;

int main(int argc, char **argv) {
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;

    FILE* readfp;
    FILE* writefp;

    if(argc != 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        perror("socket() error!");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connnect() error");
        exit(1);
    }
    else {
        puts("connect");
    }

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while(1) {
        fputs("input message(Q to quit):", stdout);

        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        fputs(message, writefp);
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        printf("message from server: %s", message);
    }
    fclose(writefp);
    fclose(readfp);

    return 0;
}