#include "chat.h"

void* listening_msg(void* arg) {
  key_t key_semaphore_client, key_client;
  int semid_client, shmid_client, sem_value;
  message *msg_request;
  char *name = (char*)arg;

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

  msg_request = shmat(shmid_client, 0, 0);  // сообщения с ответом всем клиентам
  if (msg_request == NULL) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  while (1) {
    sem_value = semctl(semid_client, 0, GETVAL, 0);
    printf("%s : %s\n", msg_request->mname, name);
    if (!strcmp(msg_request->mname, name)) {
      semctl(semid_client, 0, SETVAL, --sem_value);
      sleep(1);
      continue;
    }
    if (sem_value > 0) {
      printf("%s : %s\n", msg_request->mname, msg_request->mtext);
      semctl(semid_client, 0, SETVAL, --sem_value);
      sleep(1);
    }
  }

  return NULL;
}

void* send_msg(void* arg) {
  char *name = (char*)arg, buf[128];
  key_t key_server, key_semaphore;
  int shmid_server, semid;
  message* msg;

  key_semaphore = ftok(SEM_NAME_SERVER, SEM_NUM_SERVER);
  if (key_semaphore == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  semid = semget(key_semaphore, 1, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
  if (semid == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  key_server = ftok(SERVER_NAME, SERVER_NUM);
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

  msg = shmat(shmid_server, 0, 0);
  if (msg == NULL) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  fflush(stdin);
  sync();
  while (fgets(buf, 128, stdin)) {
    while (semctl(semid, 0, GETVAL, 0) || msg->mtype != MESSAGE_TYPE_EMPTY)
      ;
    semctl(semid, 0, SETVAL, 1);
    strncpy(msg->mtext, buf, 128);
    strcpy(msg->mname, name);
    msg->mtype = MESSAGE_TYPE_STRING;
    semctl(semid, 0, SETVAL, 0);
    fflush(stdin);
    sync();
  }
  return NULL;
}

int main() {
  pthread_t thread_listen, thread_send;
  pthread_attr_t attr;
  char name[20];

  printf("Enter name(20 max): ");
  scanf("%s", name);
  fflush(stdin);
  sync();

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