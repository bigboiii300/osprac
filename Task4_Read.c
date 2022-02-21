#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
int fd;
size_t size;
char result[13]; 
char file_name[] = "read_write.fifo";

if(mknod(file_name, S_IFIFO | 0666, 0) < 0){
	printf("Can't create FIFO!\n");
	exit(-1);
}

printf("Now, I start to read from FIFO!\n");

if((fd = open(file_name, O_RDONLY)) < 0){
	printf("Can't open FIFO to read data!");
	exit(-1);
}

size = read(fd, result, 13);
if(size!=13){
	printf("Can't read data!");
	exit(-1);
}

printf("I have read from FIFO: %s\n", result);
close(fd);

return 0;
}
