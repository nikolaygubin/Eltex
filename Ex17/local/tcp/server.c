#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>


#define BUF_SIZE 100
#define SOCKNAME "/tmp/mysocket"
#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0);

int main() {
	int sockfd, clfd;
	struct sockaddr_un addr;	
	char buf[BUF_SIZE];

	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd == -1)
		handle_error("socket");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct  sockaddr_un)) == -1)
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














