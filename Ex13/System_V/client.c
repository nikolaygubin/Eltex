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

struct msgbuf { // структура сообщения
  long mtype;
  char mtext[MSG_SIZE];
};

int main() {
  int msgqid_server, msgqid_client, msg_len;
  key_t key_server, key_client;
  struct msgbuf response, request;

  key_server = ftok(".", 1); // ключ сервера
  if (key_server == -1) {
    perror("ftok server");
    exit(EXIT_FAILURE);
  }

  msgqid_server = msgget(key_server, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT); // получаем идентификатор сервера
  if (msgqid_server == -1) {
    perror("msgget server");
    exit(EXIT_FAILURE);
  }

  key_client = ftok(".", 2); // ключ клиента
  if (key_client == -1) {
    perror("ftok client");
    exit(EXIT_FAILURE);
  }

  msgqid_client = msgget(key_client, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT); // получаем идентификатор клиента
  if (msgqid_client == -1) {
    perror("msgget client");
    exit(EXIT_FAILURE);
  }

  while (1) {   // в бесконечном цикле пытаемся считать сообщения в очереди клиента
    if ((msg_len = msgrcv(msgqid_client, &request, MSG_SIZE, 1, 0)) == -1) {
      if (errno == EINTR) continue;
      perror("msgrcv");
      break;
    }
    printf("client request : %s\n", request.mtext);
    break;
  }

  response.mtype = 1;

  if (!strcmp(request.mtext, "Hello, bro!")) {
    memcpy(response.mtext, "Good afternoon!", strlen("Good afternoon!") + 1);
  }
  if (!strcmp(request.mtext, "Goodbay, bro!")) {
    memcpy(response.mtext, "Goodbay!", strlen("Goodbay!") + 1);
  }

  if (msgsnd(msgqid_server, &response, MSG_SIZE, IPC_NOWAIT)) { // отправляем ответ на сервер
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }

  if (msgctl(msgqid_client, IPC_RMID, NULL) == -1) { // удаляем очередь
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}