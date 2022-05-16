#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>


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
    key_t  key;
    
    int pidFile = open("11-3b.c", O_CREAT | O_RDWR, 0666);
    int rc = flock(pidFile, LOCK_EX | LOCK_NB);
    if (!rc) {
        if ((key = ftok(pathname, 0)) < 0) {
            printf("Can't generate key\n");
            exit(-1);
        }

        if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
            printf("Can't get msqid\n");
            exit(-1);
        }

        while (1) {
            if (msgrcv(msqid, (struct clientbuffer*)&clientbuffer, sizeof(clientbuffer.info), -2, 0) < 0) {
                printf("Can't receive message from queue\n");
                exit(-1);
            }

            if (clientbuffer.mtype == 2) {
                printf("It doesn't work!\n");
                exit(-1);
            }

            printf("Client pid = %d, his message: %f\n", clientbuffer.info.pid, clientbuffer.info.message);

            serverbuffer.mtype = clientbuffer.info.pid;
            serverbuffer.info.message = clientbuffer.info.message * clientbuffer.info.message;

            if (msgsnd(msqid, &serverbuffer, sizeof(serverbuffer.info), 0) < 0) {
                printf("Can't send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
                exit(-1);
            }

            printf("Server send something to client!\n");

        }
    }
    else {
        printf("Server is running!\n");
    }
    return 0;
}
