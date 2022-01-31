#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
pid_t pid = fork();

if (pid == -1)
	printf("Something is wrong, there is no child process!\n");
else if (pid == 0)
	printf("I'm child process with P_Id = %d\nMy parent's PID = %d\n\n", (int)getpid(), 		(int)getppid());
else {
	printf("I'm parent process with P_Id = %d\nMy child's PID = %d\n\n", (int)getpid(), 		(int)pid);
	sleep(1);
}
return 0;
}
