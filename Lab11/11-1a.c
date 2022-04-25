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
  char pathname[]="11-1a.c"; 
  key_t  key;     
  int i,len;
  
  struct mymsgbuf { // Message structure with string and short fields.
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
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */
  strcpy(mybuf.info.txt, "Hello");
  mybuf.mtype = 1;
  for (i = 1; i <= 4; i++) {
    // 3е и 4е сообщения будут другого типа
    if (i>=3){
		strcpy(mybuf.info.txt, "");
    	mybuf.mtype = 2;
      mybuf.info.num = i;
    }
    
    len = sizeof(mybuf.info)+1;
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  /* Send the last message */
  mybuf.mtype = LAST_MESSAGE;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}
