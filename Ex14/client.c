#include "chat.h"

void* send_msg(void* arg) {
  int msgqid_server;
  key_t key_server;
  struct msg_buf message;
  char* name = (char*)arg;

  if ((key_server = ftok(SERVER_NAME, SERVER_NUM)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  if ((msgqid_server =
           msgget(key_server, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT)) == -1) {
    perror("msg_get");
    exit(EXIT_FAILURE);
  }

  message.mtype = 1;
  memcpy(message.mname, name, strlen(name) + 1);
  strcat(message.mtext, "New user! Welcome, ");
  strcat(message.mtext, name);

  printf("\n\nCHAT HISTORY:\n");
  if ((msgsnd(msgqid_server, &message, MSG_SIZE, IPC_NOWAIT)) == -1) {
    perror("msgsnd");
    exit(EXIT_FAILURE);
  }
  fflush(stdin);
  sync();

  while (fgets(message.mtext, 128, stdin) != NULL) {
    if ((msgsnd(msgqid_server, &message, MSG_SIZE, IPC_NOWAIT)) == -1) {
      perror("msgsnd");
      return NULL;
    }
    fflush(stdin);
    sync();
  }
  return NULL;
}

void* listening_msg(void* arg) {
  int msgqid_client;
  key_t key_client;
  struct msg_buf message;
  char* name = (char*)arg;

  if ((key_client = ftok(CLIENT_NAME, CLIENT_NUM)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  if ((msgqid_client =
           msgget(key_client, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT)) == -1) {
    perror("msg_get");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if (msgrcv(msgqid_client, &message, MSG_SIZE, 1, 0) == -1) {
      if (errno == EINTR) continue;
      perror("msgrcv");
      return NULL;
    }
    if (!strcmp(message.mname, name)) continue;
    printf("%s: %s", message.mname, message.mtext);
  }
  return NULL;
}

int main() {
  pthread_t thread_listen, thread_send;
  pthread_attr_t attr;
  char name[20];

  printf("Enter name(20 max): ");
  scanf("%s", name);

  pthread_attr_init(&attr);
  if (pthread_create(&thread_listen, &attr, listening_msg, (void*)name) == 1) {
    perror("pthread_create listen");
    exit(EXIT_FAILURE);
  }
  if (pthread_create(&thread_send, &attr, send_msg, (void*)name) == 1) {
    perror("pthread_create send");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(thread_send, NULL) == -1) {
    perror("thread_join send");
    exit(EXIT_FAILURE);
  }
  if (pthread_join(thread_listen, NULL) == -1) {
    perror("thread_join listen");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}