#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[], char *envp[]){
	printf("Number of command line arguments - %d\n", argc);
	printf("Command line arg:\n");
	for(int i=0; i<argc; i++)
		printf("Argument %d: %s\n", i+1, argv[i]);
		
	printf("\nEnvironment params:\n");
	int index = 0;
	while(envp[index]!=NULL){
		printf("%s\n", envp[index++]);
	}
	printf("\n");
}
