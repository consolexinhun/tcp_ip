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

const int BUF = 30;

int main() {
    FILE* fp;
    int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);

    if (fd == -1) {
        perror("file open error");
        exit(1);
    }

    fp = fdopen(fd, "w");
    fputs("network c programming \n", fp);
    fclose(fp);
    return 0;
}
