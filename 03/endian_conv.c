#include<stdio.h>
#include<arpa/inet.h>

int main() {
    unsigned short host_port = 0x1234;
    unsigned short net_port;

    unsigned long host_addr=0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("host order port: %#x\n", host_port);
    printf("network order port: %#x\n", net_port);

    printf("host order address: %#x\n", host_addr);
    printf("network order address: %#x\n", net_addr);

    // printf("%d\n", sizeof(long)); // 8
    return 0;
}