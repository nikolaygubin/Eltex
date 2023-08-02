#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef struct {
  int mtype;
  int client_fd;
} msg;

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0);

#define START_MES "Hello, I'm ready to work!"
#define BUF_SIZE 1024

#define PORT 8080

#define MAX_NUM_CLIENTS 900

void *process(void *arg) {
  int msgqid = *((int *)arg);

  while (1) {
    msg message;
    char buf[BUF_SIZE];

    if (msgrcv(msgqid, &message, sizeof(msg), 0, 0) == -1) {
      if (errno == EINTR) continue;
      perror("msgrcv");
      break;
    }

    if (send(message.client_fd, START_MES, BUF_SIZE, 0) == -1)
      handle_error("send");

    if (recv(message.client_fd, buf, BUF_SIZE, 0) == -1) handle_error("recv");
    printf("msg from cleint : %s\n", buf);
  }

  return NULL;
}

int main() {
  int sockfd, clfd;
  int msgqid;
  key_t key;
  struct sockaddr_in server;
  socklen_t addr_len = sizeof(server);
  pthread_t client_threads[MAX_NUM_CLIENTS];
  pthread_attr_t attr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) handle_error("socket");

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) ==
      -1)
    handle_error("bind");

  if (listen(sockfd, 5) == -1) handle_error("listen");

  key = ftok("server", 123);
  if (key == -1) handle_error("ftok");

  msgqid = msgget(key, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid == -1) handle_error("msgget");

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  for (int i = 0; i < MAX_NUM_CLIENTS; i++) {
    pthread_create(&client_threads[i], &attr, process, &msgqid);
  }

  while (1) {
    clfd = accept(sockfd, (struct sockaddr *)&server, &addr_len);
    if (clfd == -1) handle_error("accept");

    msg *message = malloc(sizeof(msg));
    message->mtype = 1;
    message->client_fd = clfd;

    if (msgsnd(msgqid, message, sizeof(msg), IPC_NOWAIT) == -1)
      handle_error("msgsnd");

    free(message);
  }

  if (msgctl(msgqid, IPC_RMID, NULL) == -1) handle_error("msgctl");

  exit(EXIT_SUCCESS);
}