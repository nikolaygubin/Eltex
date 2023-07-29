#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) do { perror(msg); exit (EXIT_FAILURE);} while (0);
#define BUF_SIZE 100
#define SOCK_NAME "socket_server"
#define SOCK_NAME_CLIENT "socket_client"

int main() {
	int sockfd;
	struct sockaddr_un server, client;
	char buf[BUF_SIZE] = "salam";
	socklen_t addr_len = sizeof(client.sun_family) + strlen(SOCK_NAME);

	sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		handle_error("socket");
	}

	memset(&client, 0, sizeof(struct sockaddr_un));
	client.sun_family = AF_LOCAL;
	strncpy(client.sun_path, SOCK_NAME_CLIENT, sizeof(client.sun_path) - 1);

	if (bind(sockfd, (struct sockaddr *)&client, addr_len) == -1) {
		handle_error("bind");
	}

	memset(&server, 0, sizeof(struct sockaddr_un));
	server.sun_family = AF_LOCAL;
	strncpy(server.sun_path, SOCK_NAME, sizeof(server.sun_path) - 1);

	sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server, addr_len);

	memset(buf, 0, BUF_SIZE);
	recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server, &addr_len);
	printf("Message from server: %s\n", buf);

	close(sockfd);	
	remove(SOCK_NAME_CLIENT);

	exit(EXIT_SUCCESS);
}





