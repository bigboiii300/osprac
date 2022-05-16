#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


struct client
{
    long mtype;
    struct
    {
        pid_t pid;
        float message;
    } info;
} clientbuffer;

struct server
{
    long mtype;
    struct {
        float message;
    } info;
} serverbuffer;


int main(void)
{
    char pathname[] = "11-3b.c";
    int msqid, len, maxlen;
    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    clientbuffer.mtype = 2;
    clientbuffer.info.message = 0;
    clientbuffer.info.pid = getpid();

    printf("Stop server!");

    len = sizeof(clientbuffer.info);
    if (msgsnd(msqid, &clientbuffer, len, 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }


    return 0;
}
