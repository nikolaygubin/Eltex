#include "chat.h"

int main() {
  key_t key_server, key_client, key_semaphore_server, key_semaphore_client;
  int shmid_server, shmid_client, semid_server, semid_client, count_users = 0;
  message *msg_response, *msg_request;
  char **users = malloc(sizeof(char **) * 10);

  key_semaphore_server =
      ftok(SEM_NAME_SERVER, SEM_NUM_SERVER);  // семафора для сервера
  if (key_semaphore_server == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  semid_server =
      semget(key_semaphore_server, 1, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
  if (semid_server == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  key_semaphore_client =
      ftok(SEM_NAME_CLIENT, SEM_NUM_CLIENT);  // семафора для клиентта
  if (key_semaphore_client == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  semid_client =
      semget(key_semaphore_client, 1, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
  if (semid_client == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  key_server = ftok(SERVER_NAME, SERVER_NUM);  // идентификатор для сервера
  if (key_server == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  shmid_server = shmget(key_server, sizeof(message),
                        IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
  if (shmid_server == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  key_client = ftok(CLIENT_NAME, CLIENT_NUM);  // идентификатор для клиента
  if (key_client == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }
  shmid_client = shmget(key_client, sizeof(message),
                        IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
  if (shmid_client == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  msg_response = shmat(shmid_server, 0, 0);  // сообщение с запросом от клиента
  if (msg_response == NULL) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  msg_request = shmat(shmid_client, 0, 0);  // сообщения с ответом всем клиентам
  if (msg_request == NULL) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  semctl(semid_server, 0, SETVAL, 0);
  msg_response->mtype = MESSAGE_TYPE_EMPTY;

  while (1) {
    if (msg_response->mtype == MESSAGE_TYPE_STRING) {
      if (semctl(semid_server, 0, GETVAL, 0)) {
        continue;
      }

      semctl(semid_server, 0, SETVAL, 1);
      int new = 1;
      for (int i = 0; i < count_users; i++) {
        if (!strcmp(msg_response->mname, users[i])) {
          new = 0;
          break;
        }
      }
      if (new) {
        char *user = malloc(20);
        strcpy(user, msg_response->mname);
        users[count_users++] = user;
        memset(msg_request->mname, '\0', 20);
        memset(msg_request->mtext, '\0', 128);
        strcpy(msg_request->mname, "SERVER INFO");
        strcat(msg_request->mtext, "New user! Welcome, ");
        strcat(msg_request->mtext, msg_response->mname);
        semctl(semid_client, 0, SETVAL, count_users);
        printf("New user : %s\n", msg_response->mname);
        // msg_response->mtype = MESSAGE_TYPE_EMPTY;
        semctl(semid_server, 0, SETVAL, 0);
        continue;
      }

      memset(msg_request->mname, '\0', 20);
      memset(msg_request->mtext, '\0', 128);
      strcpy(msg_request->mname, msg_response->mname);
      strcpy(msg_request->mtext, msg_response->mtext);
      semctl(semid_client, 0, SETVAL, count_users);
      printf("%s : %s", msg_response->mname, msg_response->mtext);

      msg_response->mtype = MESSAGE_TYPE_EMPTY;
      semctl(semid_server, 0, SETVAL, 0);
    }
  }

  if (semctl(semid_server, 0, IPC_RMID, (struct semid_server_ds *)0) < 0) {
    perror("semctl");
    exit(EXIT_FAILURE);
  }
  shmdt(msg_response);
  if (shmctl(shmid_server, IPC_RMID, (struct shmid_ds *)0) < 0) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }

  if (semctl(semid_client, 0, IPC_RMID, (struct semid_server_ds *)0) < 0) {
    perror("semctl");
    exit(EXIT_FAILURE);
  }
  shmdt(msg_request);
  if (shmctl(shmid_client, IPC_RMID, (struct shmid_ds *)0) < 0) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < count_users; i++) {
    free(users[i]);
  }

  exit(EXIT_SUCCESS);
}