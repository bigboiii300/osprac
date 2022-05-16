#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 

int main(void)
{
  int msqid;
  char pathname[]="11-2a.c";
  key_t  key; 
  int i,len;     

  struct mymsgbuf
  {
    long mtype;
    char mtext[81];
  } 
  mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can't generate key\n");
    exit(-1);
  }
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can't get msqid\n");
    exit(-1);
  }
   // Send message
   mybuf.mtype = 1;
   strcpy(mybuf.mtext, "Hello, it's me!");
   len = strlen(mybuf.mtext)+1;

   if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
     printf("Can't send message to queue\n");
     msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
     exit(-1);
   }
  
  
  int maxlen = 81;
  // Получаем сообщения только типа 2
  if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0)) < 0) {
    printf("Can't receive message from queue\n");
    exit(-1);
  }

  printf("I received: %s\n", mybuf.mtext);

  return 0;
}
