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
        sleep(15);
        return 24;
    } else {
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            puts("sleep 3 second");
        }

        if (WIFEXITED(status)) {
            printf("child process send %d \n", WEXITSTATUS(status));
        }
    }
    return 0;
}
