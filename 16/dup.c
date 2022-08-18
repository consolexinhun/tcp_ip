#include <stdio.h>
#include <unistd.h>

int main() {
    int fd1, fd2;
    char str1[] = "hi \n";
    char str2[] = "it is nice day \n";

    fd1 = dup(1);
    fd2 = dup2(fd1, 7);

    printf("fd1 = %d fd2 = %d \n", fd1, fd2);
    write(fd1, str1, sizeof(str1));
    write(fd2, str2, sizeof(str2));

    close(fd1);
    close(fd2);

    write(1, str1, sizeof(str1));
    close(1);
    write(1, str2, sizeof(str2));
    return 0;
}
