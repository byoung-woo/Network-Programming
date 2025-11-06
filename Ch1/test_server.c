#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main (int argc, char *argv[]){

	int sock_serv;
	int sock_clnt;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	int str_len;
	char message[BUF_SIZE];

	if(argc != 2){
		fprintf(stderr, "Usage %s <Port>\n", argv[0]);
		exit(1);
	}

	sock_serv = socket(PF_INET, SOCK_STREAM, 0);
	if(sock_serv == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sock_serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error!");
	}

	if(listen(sock_serv, 5) == -1){
		error_handling("listen() error!");
	}

	for(int i =0; i < 5; i++){

		sock_clnt = accept(sock_serv, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if(sock_clnt == -1){
			error_handling("accept() error!");
		}
		
		while((str_len = read(sock_clnt, message, BUF_SIZE)) != 0 ){
			write(sock_clnt, message, str_len);
		}
		close(sock_clnt);
	}
	close(sock_serv);
	

	return 0;

}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}