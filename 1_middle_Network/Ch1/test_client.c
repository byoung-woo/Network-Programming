#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]){

    int sock;
    struct sockaddr_in sock_serv;

    char message[BUF_SIZE];
    int str_len;

    if(argc != 3){
        fprintf(stderr, "Usage %s <IP> <Port>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&sock_serv, 0, sizeof(sock_serv));
    sock_serv.sin_family = AF_INET;
    sock_serv.sin_addr.s_addr = inet_addr(argv[1]);
    sock_serv.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&sock_serv, sizeof(sock_serv)) == -1) {
        error_handling("connect() error");
    }

    while(1){

        fputs("input message : ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE-1);
        message[str_len] = 0;
        printf("%s", message);

    }

    

  
    
    close(sock);

    return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}