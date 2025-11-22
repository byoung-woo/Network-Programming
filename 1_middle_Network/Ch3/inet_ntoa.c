#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x01020304);   // 1.2.3.4
    addr2.sin_addr.s_addr = htonl(0x01010101);   // 1.1.1.1

    str_ptr = inet_ntoa(addr1.sin_addr);         // 1.2.3.4
    strcpy(str_arr, str_ptr);                    // 복사해둠
    printf("Dotted-Decimal notation1: %s \n", str_ptr);   // 1.2.3.4

    str_ptr = inet_ntoa(addr2.sin_addr);         // 1.1.1.1
    printf("Dotted-Decimal notation2: %s \n", str_ptr);   // 1.1.1.1
    printf("Dotted-Decimal notation3: %s \n", str_arr);   // 1.2.3.4 (복사해둔 값)

    return 0;
}
