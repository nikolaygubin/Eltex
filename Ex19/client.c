#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 7777
#define IP "127.0.0.9"
#define BUF_SIZE 256

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

int main() {
  int sockfd;
  char buf[BUF_SIZE];
  struct sockaddr_in server, client;
  socklen_t server_size = sizeof(server), client_size = sizeof(client);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(IP);

  client.sin_family = AF_INET;
  client.sin_port = htons(7070);
  client.sin_addr.s_addr = inet_addr("127.0.0.11");

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sockfd == -1) handle_error("socket");

  if (connect(sockfd, (struct sockaddr *)&server, server_size) == -1)
    handle_error("connect");

  memset(buf, 0, BUF_SIZE);
  strcpy(buf, "Who is here?");
  if (sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&client,
             client_size) == -1)
    handle_error("sendto");

    if (recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server,
                 &client_size) == -1)
      handle_error("recvfrom");
  printf("server message : %s\n", buf);

  strcat(buf, "I'm here too!");
  if (sendto(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&server,
             server_size) == -1)
    handle_error("sendto");

  exit(EXIT_SUCCESS);
}