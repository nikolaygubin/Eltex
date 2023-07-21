## POSIX
Link with -lrt.

#include <mqueue.h>

mqd_t mq_open(const char *name, int oflag);

mqd_t mq_open(const char *name, int oflag, mode_t mode,
              struct mq_attr *attr);

int mq_close(mqd_t mqdes);

int mq_unlink(const char *name);

int mq_send(mqd_t mqdes, const char *msg_ptr,
            size_t msg_len, unsigned int msg_prio);

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,
                   size_t msg_len, unsigned int *msg_prio);

struct mq_attr {
    
    long mq_flags; // 0 or O_NONBLOCK (mqgetattr(), mq_setattr())

    long mq_maxmsg; // max num of msg queue

    long mq_msgsize; // max msgsize in bytes

    long cur_msgs; // number of msgs currently in queue
}


## System V

#include <sys/ipc.h>
#include <sys/msg.h>

key_t ftok(const char *pathname, int proj_id);

int msgget(key_t key, int msgflg);

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,
               int msgflg);

int msgctl(int msqid, int cmd, struct msqid_ds *buf);

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */
};

struct msqid_ds {
    struct ipc_perm msg_perm;   /* Ownership and permissions */
    time_t          msg_stime;  /* Time of last msgsnd(2) */
    time_t          msg_rtime;  /* Time of last msgrcv(2) */
    time_t          msg_ctime;  /* Time of creation or last
                                   modification by msgctl() */
    unsigned long   msg_cbytes; /* # of bytes in queue */
    msgqnum_t       msg_qnum;   /* # number of messages in queue */
    msglen_t        msg_qbytes; /* Maximum # of bytes in queue */
    pid_t           msg_lspid;  /* PID of last msgsnd(2) */
    pid_t           msg_lrpid;  /* PID of last msgrcv(2) */
};

## modes

S_IRWXU  00700 user (file owner) has read, write, and execute permission

S_IRUSR  00400 user has read permission

S_IWUSR  00200 user has write permission

S_IXUSR  00100 user has execute permission

S_IRWXG  00070 group has read, write, and execute permission

S_IRGRP  00040 group has read permission

S_IWGRP  00020 group has write permission

S_IXGRP  00010 group has execute permission

S_IRWXO  00007 others have read, write, and execute permission

S_IROTH  00004 others have read permission

S_IWOTH  00002 others have write permission

S_IXOTH  00001 others have execute permission


According to POSIX, the effect when other bits are set in mode is unspecified.  On Linux, the following bits are also honored in mode:

S_ISUID  0004000 set-user-ID bit

S_ISGID  0002000 set-group-ID bit (see inode(7)).

S_ISVTX  0001000 sticky bit (see inode(7)).