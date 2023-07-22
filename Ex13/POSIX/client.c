#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 256
#define MAX_MSG 10
#define SERVER_NAME "/server_q"
#define CLIENT_NAME "/client_q"

int main() {
  char buf[MSG_SIZE];
  unsigned priority;
  mqd_t server_queue, client_queue;
  struct mq_attr attribute;
  int msg_len;

  attribute.mq_flags = 0;
  attribute.mq_maxmsg = MAX_MSG;
  attribute.mq_msgsize = MSG_SIZE;
  attribute.mq_curmsgs = 0;

  server_queue = mq_open(SERVER_NAME, O_CREAT | O_RDWR | O_NONBLOCK,
                         S_IRWXU | S_IRWXG | S_IRWXO, &attribute);

  client_queue = mq_open(CLIENT_NAME, O_CREAT | O_RDWR | O_NONBLOCK,
                         S_IRWXU | S_IRWXG | S_IRWXO, &attribute);

  if (server_queue == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if ((msg_len = mq_receive(client_queue, buf, MSG_SIZE, &priority)) == -1) {
      if (errno = EINTR) continue;
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    printf("Server msg text : %s\n", buf);
    break;
  }

  if (mq_send(server_queue, "Hello!, server!", strlen("Hello, server!") + 1,
              0) == -1) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }

  if (mq_close(server_queue) == -1) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }

  if (mq_close(client_queue) == -1) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }

  if (mq_unlink(CLIENT_NAME) == -1) {
    perror("mq_unlink");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}