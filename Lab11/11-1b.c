#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid, len, maxlen;
  char pathname[]="11-1a.c";
  key_t  key;

  struct mymsgbuf { // Message structure with string and short fields
  long mtype;
  struct {
    short num;
    char txt[81];
    } 
    info;
  } 
  mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  while (1) {
    // In an infinite loop, accept messages of any type in FIFO order with a maximum length of 81 characters
    // until a message of type LAST_MESSAGE is received.
    maxlen = sizeof(mybuf.info)+1;
    
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    
    // If the received message is of type LAST_MESSAGE, then terminate and remove the message queue from the system
    // Otherwise, print the text of the received message
    if (mybuf.mtype == LAST_MESSAGE) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }
	 
	 if (mybuf.mtype == 1)
      printf("message type = %ld, msg = %s\n", mybuf.mtype, mybuf.info.txt);
    else if (mybuf.mtype == 2)
     printf("message type = %ld, num = %d\n", mybuf.mtype, mybuf.info.num);
  }

  return 0;
}
