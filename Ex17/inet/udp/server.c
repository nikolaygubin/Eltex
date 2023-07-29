#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define BUF_SIZE 100
#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0);
#define PORT 8080
#define HOST "127.0.0.1"

int main() {
	int sockfd;
	struct sockaddr_in serv, client;	
	char buf[BUF_SIZE];
	socklen_t addr_len = sizeof(client);

	sockfd = socket(AF_INET, SOCK_DGRAM,  0);
	if (sockfd == -1)
		handle_error("socket");

	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	if (inet_aton(HOST, &serv.sin_addr) == 0) {
		fprintf(stderr, "invalid adress!\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sockfd, (struct sockaddr *)&serv, sizeof(struct  sockaddr_in)) == -1)
		handle_error("bind");
	
	if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&client, &addr_len) == -1) {
		handle_error("recvfrom");
	}

	printf("Message from client : %s\n", buf);

	if (sendto(sockfd, "salam aleicum brat", BUF_SIZE, 0, (struct sockaddr *)&client, addr_len) == -1) {
		handle_error("sendto");
	}	

	close(sockfd);
	
	exit(EXIT_SUCCESS);
}














