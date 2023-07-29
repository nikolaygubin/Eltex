#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 100
#define SOCKNAME "socket_server"
#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0);

int main() {
	int sockfd;
	struct sockaddr_un serv, client;	
	char buf[BUF_SIZE];
	socklen_t addr_len = sizeof(client.sun_family) + strlen(SOCKNAME);

	sockfd = socket(AF_LOCAL, SOCK_DGRAM,  0);
	if (sockfd == -1)
		handle_error("socket");

	memset(&serv, 0, sizeof(struct sockaddr_un));
	serv.sun_family = AF_LOCAL;
	strncpy(serv.sun_path, SOCKNAME, sizeof(serv.sun_path) - 1);

	if (bind(sockfd, (struct sockaddr *)&serv, sizeof(struct  sockaddr_un)) == -1)
		handle_error("bind");
	
	if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&client, &addr_len) == -1) {
		handle_error("recvfrom");
	}

	printf("Message from client : %s\n", buf);

	if (sendto(sockfd, "salam aleicum brat", BUF_SIZE, 0, (struct sockaddr *)&client, addr_len) == -1) {
		handle_error("sendto");
	}	

	close(sockfd);
	remove(SOCKNAME);
	
	exit(EXIT_SUCCESS);
}














