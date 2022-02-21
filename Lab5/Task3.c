#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>


int main(){
int fd[2];
size_t size = 1;
char* byte = malloc(sizeof(char));
int result = 0;

if(pipe(fd) < 0){
	printf("Can't create pipe!");
	exit(-1);
}

fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

while(size == 1){
	size = write(fd[1], byte, 1);
	result++;
}

printf("Size of buffer = %d bytes\n", result);

free(byte);
return 0;
}
