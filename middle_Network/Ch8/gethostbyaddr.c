#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int i;
    struct hostent *host;
    struct sockaddr_in addr;
    // struct in_addr addr; // 주석 처리되어 있지만, IP 주소 저장을 위해 struct in_addr를 직접 사용해야 합니다.

    if (argc != 2)
    {
        printf("Usage : %s <IP>\n", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    // IP 주소 문자열을 in_addr 구조체로 변환
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    // gethostbyaddr 함수 호출:
    // (char*)&addr.sin_addr : 주소 정보를 담은 in_addr 구조체의 포인터
    // 4 : IP 주소의 길이 (IPv4는 4바이트)
    // AF_INET : 주소 체계 (IPv4)
    host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
    // host = gethostbyaddr((char *)&addr, 4, AF_INET); // 주석 처리된 라인은 sockaddr_in 구조체 전체를 넘기려는 시도로 보이나, 정확히는 sin_addr 멤버를 넘겨야 합니다.

    if (!host)
    {
        error_handling("gethost... error");
    }

    printf("Official name: %s \n", host->h_name);

    for (i = 0; host->h_aliases[i]; i++)
    {
        printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
    }

    printf("Address type: %s \n",
           (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

    for (i = 0; host->h_addr_list[i]; i++)
    {
        printf("IP addr %d: %s \n", i + 1,
               inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
    }

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}