#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

int main() {
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x01020304);
    addr2.sin_addr.s_addr = htonl(0x01010101);

    str_ptr = inet_ntoa(addr1.sin_addr); // 网络字节序32位整数IP 转 字符串
    strcpy(str_arr, str_ptr);
    printf("notation1 :%s\n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("notation2: %s\n", str_ptr);

    printf("notation3: %s\n", str_arr);
    return 0;
}