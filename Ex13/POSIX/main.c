#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 256
#define MAX_MSG 10

int main() {
  const char name[] = "/msg_queue", message[MSG_SIZE] = "Hello, bro!";
  char buf[MSG_SIZE];
  unsigned priority;
  mqd_t queue_fd;
  struct mq_attr attribute;

  attribute.mq_flags = 0;
  attribute.mq_maxmsg = MAX_MSG;
  attribute.mq_msgsize = MSG_SIZE;
  attribute.mq_curmsgs = 0;

  queue_fd = mq_open(name, O_CREAT | O_RDWR | O_NONBLOCK,
                     S_IRWXU | S_IRWXG | S_IRWXO, &attribute);

  if (queue_fd == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 10; i++) {
    if (mq_send(queue_fd, message, strlen(message) + 1, 0) == -1) {
      perror("mq_send");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < 10; i++) {
    if (mq_receive(queue_fd, buf, 300, &priority) == -1) {
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }

    printf("%s\n", buf);
  }

  if (mq_close(queue_fd) == -1) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }

  if (mq_unlink(name) == -1) {
    perror("mq_unlink");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}