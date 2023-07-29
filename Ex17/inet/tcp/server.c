#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUF_SIZE 100
#define SOCKNAME "/tmp/mysocket"
#define HOST "127.0.0.1"
#define PORT 8080
#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0);

int main() {
	int sockfd, clfd;
	struct sockaddr_in addr;	
	char buf[BUF_SIZE];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		handle_error("socket");

	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	if (inet_aton(HOST, &addr.sin_addr) == -1) {
		fprintf(stderr, "invalid adress!\n");
		exit(EXIT_FAILURE);
	}
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct  sockaddr_in)) == -1)
		handle_error("bind");

	if (listen(sockfd, 5) == -1)
		handle_error("listen");

	clfd = accept(sockfd, NULL, NULL);
	if (clfd == -1)
		handle_error("accept");

	if (recv(clfd, buf, BUF_SIZE, 0) == -1) {
		handle_error("recv");
	}

	printf("Message from client : %s\n", buf);

	if (send(clfd, "salam aleikum bratik", BUF_SIZE, 0) == - 1) {
		handle_error("send");
	}

	close(clfd);
	close(sockfd);
	remove(SOCKNAME);

}














