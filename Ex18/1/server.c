#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/resource.h>

#define START_MES "Hello, I'm ready to work!"
#define BUF_SIZE 1024
#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);
#define PORT 8080
#define MAX_NUM_CLIENTS 1000

void *client_thread(void *arg) {
  int *fd = (int *)arg;
  char buf[BUF_SIZE];

  if (send(*fd, START_MES, BUF_SIZE, 0) == -1) handle_error("send");

  if (recv(*fd, buf, BUF_SIZE, 0) == -1) handle_error("recv");

  printf("Message from cleint: %s\n", buf);

  return arg;
}

int main() {
  int sockfd, fd;
  struct sockaddr_in server;
  pthread_t client_threads[MAX_NUM_CLIENTS];
  size_t count = 0;
  pthread_attr_t attr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) handle_error("socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    handle_error("bind");

  if (listen(sockfd, 5) == -1) handle_error("listen");

  pthread_attr_init(&attr);
  while (MAX_NUM_CLIENTS > count) {
    int client_sockfd = accept(sockfd, NULL, NULL);
    pthread_create(&client_threads[count++], &attr, client_thread,
                   &client_sockfd);
  }

  for (int i = count - 1; i >= 0; i--) {
    pthread_join(client_threads[i], (void *)&fd);
    close(fd);
  }

  close(sockfd);

  exit(EXIT_SUCCESS);
}