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
    FILE* fp1;
    FILE* fp2;
    char buf[BUF];

    fp1 = open("news.txt", "r");
    fp2 = open("cpy.txt", "w");

    while (fgets(buf, BUF, fp1) != NULL) {
        fputs(buf, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
