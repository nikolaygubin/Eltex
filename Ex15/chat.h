#ifndef CHAT_H
#define CHAT_H

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MSG_SIZE 256

#define SERVER_NAME "server"
#define CLIENT_NAME "client"
#define SERVER_NUM 100
#define CLIENT_NUM 200

#define SEM_NAME_SERVER "."
#define SEM_NUM_SERVER 333
#define SEM_NAME_CLIENT "."
#define SEM_NUM_CLIENT 666

#define MESSAGE_TYPE_EMPTY 1
#define MESSAGE_TYPE_STRING 2

typedef struct {
  long mtype;
  char mname[20];
  char mtext[128];
} message;


#endif // CHAT_H