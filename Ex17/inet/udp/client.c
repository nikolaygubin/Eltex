#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define handle_error(msg) do { perror(msg); exit (EXIT_FAILURE);} while (0);
#define BUF_SIZE 100
#define PORT 8080
#define HOST "127.0.0.1"
#define HOST_CLIENT "127.0.0.2"

int main() {
	int sockfd;
	struct sockaddr_in server, client;
	char buf[BUF_SIZE] = "salam";
	socklen_t addr_len = sizeof(client);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		handle_error("socket");
	}

	memset(&client, 0, sizeof(struct sockaddr_in));
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	if (inet_aton(HOST_CLIENT, &client.sin_addr) == 0) {
		fprintf(stderr, "invalid adress!\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sockfd, (struct sockaddr *)&client, addr_len) == -1) {
		handle_error("bind");
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	if (inet_aton(HOST, &server.sin_addr) == 0) {
		fprintf(stderr, "invalid adress!\n");
		exit(EXIT_FAILURE);
	}

	sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server, addr_len);

	memset(buf, 0, BUF_SIZE);
	recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server, &addr_len);
	printf("Message from server: %s\n", buf);

	close(sockfd);	

	exit(EXIT_SUCCESS);
}





