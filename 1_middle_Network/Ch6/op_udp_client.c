/* op_client_udp.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr, from_adr; // from_adr 추가
    socklen_t adr_sz; // from_adr의 크기

    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 1. UDP 소켓 생성 (SOCK_DGRAM)
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    // 2. connect() 함수가 사라짐

    // 사용자로부터 데이터를 입력받는 부분은 동일
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt; // 1바이트: 개수

    for(i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]); // 4바이트 * 개수: 피연산자
    }

    fgetc(stdin); // 버퍼 비우기
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); // 1바이트: 연산자

    // 3. sendto: 준비된 opmsg를 serv_adr로 한번에 전송
    sendto(sock, opmsg, opnd_cnt*OPSZ+2, 0, 
           (struct sockaddr*)&serv_adr, sizeof(serv_adr));

    // 4. recvfrom: 서버로부터 결과 수신
    adr_sz = sizeof(from_adr);
    recvfrom(sock, &result, RLT_SIZE, 0, 
             (struct sockaddr*)&from_adr, &adr_sz);

    printf("Operation result: %d \n", result);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}