#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>


int main(int argc, char** argv) {
    pid_t pid = fork();
    
    if (pid == 0) {
        puts("i am a child process");
    } else {
        printf("child process id: %d\n", pid);
        sleep(30);
    }

    if (pid == 0) {
        puts("end child process");
    } else {
        puts("end parent process");
    }
    return 0;
}
