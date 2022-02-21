#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
int fd;
size_t size;
char file_name[] = "read_write.fifo";
(void)umask(0);

if((fd=open(file_name, O_WRONLY)) < 0){
	printf("Can't open file FIFO!\n");
	exit(-1);
}

size = write(fd, "FIFO program", 13);

if(size != 13){
	printf("Can't write all strings!\n");
	exit(-1);
}

close(fd);
printf("Program for writing has finished!");

return 0;
}
