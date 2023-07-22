#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 256

struct msgbuf {
  long mtype;
  char mtext[MSG_SIZE];
};

int main() {
  int msgqid_server, msgqid_client, msg_len;
  key_t key_server, key_client;
  struct msgbuf response, request;

  key_server = ftok(".", 1);
  if (key_server == -1) {
    perror("ftok server");
    exit(EXIT_FAILURE);
  }
  msgqid_server = msgget(key_server, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid_server == -1) {
    perror("msgget server");
    exit(EXIT_FAILURE);
  }

  key_client = ftok(".", 2);
  if (key_client == -1) {
    perror("ftok client");
    exit(EXIT_FAILURE);
  }
  msgqid_client = msgget(key_client, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid_client == -1) {
    perror("msgget client");
    exit(EXIT_FAILURE);
  }

  response.mtype = 1;
  memcpy(response.mtext, "Goodbay, bro!", strlen("Goodbay, bro!") + 1);
  if (msgsnd(msgqid_client, &response, strlen(response.mtext), IPC_NOWAIT) ==
      -1) {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if ((msg_len = msgrcv(msgqid_server, &request, MSG_SIZE, 1, 0)) == -1) {
      if (errno == EINTR) continue;
      perror("msgrcv");
      break;
    }
    printf("client request : %s\n", request.mtext);
    break;
  }

  if (msgctl(msgqid_server, IPC_RMID, NULL)) {
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}