#include "chat.h"

int main() {
  int msgqid_server, msgqid_client, count_users = 0;
  key_t key_server, key_client;
  struct msg_buf response_message, request_message;
  struct msqid_ds statistic;
  // const char msg_new_user[] = "New user! Welcome, ";
  char **users = malloc(sizeof(char **) * 10);

  if ((key_server = ftok(SERVER_NAME, SERVER_NUM)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  msgqid_server = msgget(key_server, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid_server == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  if ((key_client = ftok(CLIENT_NAME, CLIENT_NUM)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  msgqid_client = msgget(key_client, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
  if (msgqid_client == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if (msgrcv(msgqid_server, (void *)&response_message, MSG_SIZE, 0, 0) ==
        -1) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("msgrcs");
        exit(EXIT_FAILURE);
      }
    } else {
      int new = 1;
      for (int i = 0; i < count_users; i++) {
        if (!strcmp(response_message.mname, users[i])) {
          new = 0;
          break;
        }
      }
      if (new) {
        char *name = malloc(20);
        strcpy(name, response_message.mname);
        users[count_users++] = name;
        if (msgctl(msgqid_client, IPC_STAT, &statistic) == -1) {
          perror("msgctl");
          exit(EXIT_FAILURE);
        }
        memset(request_message.mname, '\0', 20);
        memset(request_message.mtext, '\0', 128);
        strcpy(request_message.mname, "SERVER INFO");
        strcpy(request_message.mtext, "Append new user! Welcome, ");
        strcat(request_message.mtext, response_message.mname);
        strcat(request_message.mtext, "\n");
        request_message.mtype = 1;
        for (int i = 0; i < (int)statistic.msg_qnum + count_users; i++) {
          if (msgsnd(msgqid_client, &request_message, MSG_SIZE, IPC_NOWAIT) ==
              -1) {
            perror("msgsnd reg");
          }
        }
      } else {
        if (msgctl(msgqid_client, IPC_STAT, &statistic) == -1) {
          perror("msgctl");
          exit(EXIT_FAILURE);
        }
        request_message.mtype = 1;
        memset(request_message.mname, '\0', 20);
        memset(request_message.mtext, '\0', 128);
        strcpy(request_message.mname, response_message.mname);
        strcpy(request_message.mtext, response_message.mtext);
        for (int i = 0; i < (int)statistic.msg_qnum + count_users; i++) {
          if (msgsnd(msgqid_client, &request_message, MSG_SIZE, IPC_NOWAIT) ==
              -1) {
            perror("msgsnd");
          }
        }
      }
    }
  }

  if (msgctl(msgqid_server, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}