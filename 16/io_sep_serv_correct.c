#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;

int main(int argc, char *argv[]) {
    int serv_sock, client_sock;
    char message[BUF_SIZE];
    memset(&message, 0, sizeof(message));

    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;

    FILE* readfp;
    FILE* writefp;

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    if(listen(serv_sock, 5) == -1) {
        perror("listen() error");
        exit(1);
    }

    client_addr_size = sizeof(client_addr);
    
    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    if(client_sock == -1) {
        perror("accept() error");
    }

    readfp = fdopen(client_sock, "r");
    writefp = fdopen(dup(client_sock), "w");

    fputs("from server: client ? \n", writefp);
    fputs("i love all of the world\n", writefp);
    fputs("you are awesome \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(message, sizeof(message), readfp);
    fputs(message, stdout);
    fclose(readfp);

    close(serv_sock);
    return 0;
}