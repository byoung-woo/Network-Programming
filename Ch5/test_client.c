#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

void error_handling(char * message);

int main(int argc, char * argv[]){

    int sock_serv;
    int opnum, result;
    char opmsg[BUF_SIZE];
    struct sockaddr_in serv_addr;

    if(argc != 3){
        fprintf(stderr, "Usage %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    if((sock_serv = socket(PF_INET, SOCK_STREAM, 0)) ==-1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if((connect(sock_serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))==-1){
        error_handling("connect() error");
    }

    fputs("숫자 몇개? : ",stdout);
    scanf("%d", &opnum);
    opmsg[0] = opnum;

    for (int i = 0; i < opnum; i++){
        fputs("어떤 숫자 ? : ", stdout);
        scanf("%d", (int *)&opmsg[i*OPSZ+1]);
    }

    fgetc(stdin);
    fputs("피연산자 입력 : ",stdout);
    scanf("%c", &opmsg[opnum*OPSZ+1]);

    write(sock_serv, opmsg, opnum*OPSZ+2);
    read(sock_serv, &result, OPSZ);

    printf("Result %d\n", result);

    close(sock_serv);

    return 0;

}


void error_handling(char * message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}