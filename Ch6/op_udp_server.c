/* op_server_udp.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4 // 4바이트 (int)

void error_handling(char *message);
int calculate(int opnum, int opnds[], char op);

int main(int argc, char *argv[])
{
    int serv_sock;
    char opinfo[BUF_SIZE];
    int result, opnd_cnt, i;
    int recv_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 1. UDP 소켓 생성 (SOCK_DGRAM)
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 2. bind()는 동일하게 필요
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 3. listen()과 accept()가 사라짐
    //    대신 무한 루프로 클라이언트의 요청을 대기
    printf("UDP Operator Server started...\n");

    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        
        // 4. recvfrom: 연결 없이 데이터를 수신. clnt_adr에 클라이언트 주소가 저장됨
        recv_len = recvfrom(serv_sock, opinfo, BUF_SIZE, 0, 
                            (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        if(recv_len == -1)
            continue; // 오류 발생 시 다음 요청 대기

        // 5. 수신한 패킷에서 데이터 추출
        //    클라이언트가 [개수(1B)] + [피연산자(4B*개수)] + [연산자(1B)] 순서로 보냄
        opnd_cnt = (int)opinfo[0];
        
        // 피연산자 시작 주소: (int*)&opinfo[1]
        // 연산자 위치: opinfo[1 + opnd_cnt * OPSZ] 또는 opinfo[recv_len - 1]
        result = calculate(opnd_cnt, (int*)&opinfo[1], opinfo[opnd_cnt*OPSZ + 1]);

        // 6. sendto: 결과를 clnt_adr로 다시 전송
        sendto(serv_sock, &result, sizeof(result), 0, 
               (struct sockaddr*)&clnt_adr, clnt_adr_sz);
    }

    close(serv_sock);
    return 0;
}

int calculate(int opnum, int opnds[], char op)
{
    int result = opnds[0], i;
    switch(op)
    {
        case '+':
            for(i = 1; i < opnum; i++) result += opnds[i];
            break;
        case '-':
            for(i = 1; i < opnum; i++) result -= opnds[i];
            break;
        case '*':
            for(i = 1; i < opnum; i++) result *= opnds[i];
            break;
    }
    return result;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}