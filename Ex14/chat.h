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

#define MSG_SIZE 256
#define SERVER_NAME "server"
#define CLIENT_NAME "client"
#define SERVER_NUM 100
#define CLIENT_NUM 200

struct msg_buf {
  long mtype;
  char mname[20];
  char mtext[128];
};

#endif // CHAT_H