#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 256

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};

int main() {
    int msgqid;
    key_t key;
    struct msgbuf msg_snd, msg_rcv;
    msg_snd.mtype = 1;
    memcpy(msg_snd.mtext, "Hello, bro!", strlen("Hello, bro!") + 1);

    key = ftok(".", 1);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgqid = msgget(key, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT);
    if (msgqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgsnd(msgqid, &msg_snd, strlen(msg_snd.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgqid, &msg_rcv, MSG_SIZE, 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("msg_rcv text : %s\n", msg_rcv.mtext);

    if (msgctl(msgqid, IPC_RMID, NULL)) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}