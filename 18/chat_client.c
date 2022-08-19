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
#include <pthread.h>

#define BUF 100
#define NAME_SIZE 256

void* send_msg(void* arg);
void* recv_msg(void* arg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF];

int main(int argc, char** argv) {
    int sock;
    struct sockaddr_in addr;

    pthread_t send_thread, recv_thread;
    if (argc != 4) {
        printf("usage : %s ip port name\n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect() error");
        exit(1);
    }

    void* thread_return;
    pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(send_thread, &thread_return);
    pthread_join(recv_thread, &thread_return);
    close(sock);
    return 0;
}

void* send_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE+BUF+2];
    while (1) {
        fgets(msg, BUF, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF + 2];
    int str_len;

    while (1) {
        str_len = read(sock, name_msg, NAME_SIZE+BUF-1);
        if (str_len == -1) {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
}
