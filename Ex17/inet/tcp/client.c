#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define handle_error(msg) do { perror(msg); exit (EXIT_FAILURE);} while (0);
#define BUF_SIZE 100
#define SOCK_NAME "/tmp/mysocket"
#define HOST "127.0.0.1"
#define PORT 8080

int main() {
	int sockfd;
	struct sockaddr_in server;
	char buf[BUF_SIZE] = "salam";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		handle_error("socket");
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	if (inet_aton(HOST, &server.sin_addr) == 0) {
		fprintf(stderr, "invalid adress!\n");
		exit(EXIT_FAILURE);
	}

	if (connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
		handle_error("connect");
	}

	send(sockfd, buf, BUF_SIZE, 0);

	memset(buf, 0, BUF_SIZE);
	recv(sockfd, buf, BUF_SIZE, 0);
	printf("Message from server: %s\n", buf);

	close(sockfd);	

	exit(EXIT_FAILURE);
}





