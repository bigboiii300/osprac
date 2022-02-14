#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>


int main()
{
int fd;
size_t size;
char str[14];

(void)umask(0);
if((fd = open("myfile", O_RDONLY)) < 0){
	printf("Can't open file!");
	exit(-1);
}

size = read(fd, str, 14);
if(size!=14){
	printf("Can't read all symbols!");
	exit(-1);
}

printf("%s\n", str);
if(close(fd) < 0){
	printf("Can't close file!");
}

return 0;
}
