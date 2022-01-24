#include <stdio.h>
#include <unistd.h>

int main(int agrc,char** argv){
    printf("User Id = %d\n", getuid());
    printf("Group Id = %d\n", getgid());
    return 0;
}
