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
    
    struct{
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
    int msqid, len, maxlen;;
    key_t key;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    clientbuffer.mtype = 1;
    clientbuffer.info.pid = getpid();

    float msg;
    printf("Please, enter fractional number: ");
    scanf("%f", &msg);
    clientbuffer.info.message = msg;

    printf("Client send from type = %d message = '%f'\n", clientbuffer.mtype, clientbuffer.info.message);

    len = sizeof(clientbuffer.info);
    if (msgsnd(msqid, &clientbuffer, len, 0) < 0) {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
    }

    printf("Give me some seconds, I need to get answer from server!\n");

    if (msgrcv(msqid, &serverbuffer, sizeof(serverbuffer.info), getpid(), 0) < 0) {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }
    printf("Answer from server: %f\n", serverbuffer.info.message);


    return 0;
}
