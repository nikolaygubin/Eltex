#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) do { perror(msg); exit (EXIT_FAILURE);} while (0);
#define BUF_SIZE 100
#define SOCK_NAME "/tmp/mysocket"

int main() {
	int sockfd;
	struct sockaddr_un addr;
	char buf[BUF_SIZE] = "salam";

	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd == -1) {
		handle_error("socket");
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SOCK_NAME, sizeof(addr.sun_path) - 1);

	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
		handle_error("connect");
	}

	send(sockfd, buf, BUF_SIZE, 0);

	memset(buf, 0, BUF_SIZE);
	recv(sockfd, buf, BUF_SIZE, 0);
	printf("Message from server: %s\n", buf);

	close(sockfd);	

	exit(EXIT_FAILURE);
}





