#include <stdio.h>
#include <sys/uio.h>

#define BUF 100

int main() {
    struct iovec vec[2];
    char buf1[BUF] = {0};
    char buf2[BUF] = {0};
    int str_len;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF;

    str_len = readv(0, vec, 2);
    printf("read bytes: %d\n", str_len);
    printf("first message: %s\n", buf1);
    printf("second message: %s", buf2);
    return 0;
}