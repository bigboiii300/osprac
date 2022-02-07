#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[], char *envp[]){
	pid_t pid = fork ();
	if(pid == -1)
		printf("Smth is wrong!");
	else if(pid == 0){
		execle("/bin/bash", "bash",  "-c", "echo $PWD", 0, envp);
	}
	return 0;
}
