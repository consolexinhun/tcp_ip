#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <netdb.h>

int main(int argc, char** argv) {
    int sock;
    int send_buf, recv_buf, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(send_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&send_buf, &len);
    assert(!state);

    len = sizeof(recv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&recv_buf, &len);
    assert(!state);

    printf("input buffer size: %d\n", send_buf);
    printf("output buffer size: %d\n", recv_buf);
    return 0;
}
