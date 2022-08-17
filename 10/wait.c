#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    pid_t pid = fork();
    int status;
    if (pid == 0) {
        return 3;
    } else {
        printf("child pid: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            exit(7);
        } else {
            printf("child pid: %d\n", pid);
            wait(&status);
            if (WIFEXITED(status)) {
                printf("child send one : %d\n", WEXITSTATUS(status));
            }

            wait(&status);
            if (WIFEXITED(status)) {
                printf("child send two :%d\n", WEXITSTATUS(status));
            }

            sleep(30);
        }
    }
    return 0;
}
