#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

#define START_MES "Hello, I'm ready to work!"
#define BUF_SIZE 1024

#define PORT 8080

#define MAX_NUM_CLIENTS 10
int thread_access[MAX_NUM_CLIENTS];

void * process(void *arg) {
  int index = *((int *)arg);
  char buf[BUF_SIZE];

  while (1) {
    if (thread_access[index] != 0) {
      if (send(thread_access[index], START_MES, BUF_SIZE, 0) == -1) handle_error("send");

      if (recv(thread_access[index], buf, BUF_SIZE, 0) == -1) handle_error("recv");
      printf("Client msg : %s\n", buf);

      thread_access[index] = 0;
    }
  }

  return NULL;
}

int find_free_thread() {
  for (int i = 0; i < MAX_NUM_CLIENTS; i++) {
    if (thread_access[i] == 0) return i;
  }

  return -1;
}

int main() {
  int sockfd, clfd;
  int free_thread = -1;
  int indexes[MAX_NUM_CLIENTS];
  struct sockaddr_in server;
  socklen_t addr_len = sizeof(server);
  pthread_t client_threads[MAX_NUM_CLIENTS];
  pthread_attr_t attr;

  memset(thread_access, 0, sizeof(int) * MAX_NUM_CLIENTS);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) handle_error("socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (bind(sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) handle_error("bind");

  if (listen(sockfd, 5) == -1) handle_error("listen");

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  for (int i = 0; i < MAX_NUM_CLIENTS; i++) {
    indexes[i] = i;
    pthread_create(&client_threads[i], &attr, process, indexes + i);
  }

  while (1) {
    free_thread = find_free_thread();

    if (free_thread != -1) {
      clfd = accept(sockfd, (struct sockaddr *) &server, &addr_len);
      if (clfd != -1) {
        thread_access[free_thread] = clfd;
      }
    }
  }

  exit(EXIT_SUCCESS);
} 