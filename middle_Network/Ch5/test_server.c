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
int calculate (int opnum, int opnds[], char op);

int main(int argc, char * argv[]){

    int sock_serv, sock_clnt;
    int recv_cnt, recv_len, opnd_cnt, result;
    char opinfo[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if(argc != 2){
        fprintf(stderr, "Usage %s <Port>\n", argv[0]);
        exit(1);
    }

    if((sock_serv = socket(PF_INET, SOCK_STREAM, 0))==-1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        error_handling("bind() error");
    }

    if(listen(sock_serv, 5)==-1){
        error_handling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    for(int i =0; i<5; i++){
        sock_clnt = accept(sock_serv, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(sock_clnt == -1)
            error_handling("accept() error");


        opnd_cnt = 0;
        read(sock_clnt, &opnd_cnt, 1);

        recv_len = 0;
        while(opnd_cnt*OPSZ+1>recv_len){
            recv_cnt = read(sock_clnt, &opinfo[recv_len], BUF_SIZE -1);
            recv_len += recv_cnt;
        }
        
        result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len - 1]);
        write(sock_clnt, &result, RLT_SIZE);

        close(sock_clnt);
    }
    close(sock_serv);

    return 0;

}


int calculate (int opnum, int opnds[], char op){
    int result = opnds[0], i;

    switch(op){
        case'+': 
            for(i = 1; i < opnum; i++) result += opnds[i];
                break;
        case'-': 
            for(i = 1; i < opnum; i++) result -= opnds[i];
                break;
        case'*': 
            for(i = 1; i < opnum; i++) result *= opnds[i];
                break;
    }

    return result;
}

void error_handling(char * message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}