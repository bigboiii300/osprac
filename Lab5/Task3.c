#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
int file[2];
int full = 0;
int result = 0;
if (pipe2(file, O_NONBLOCK) < 0) {
printf("ERROR: Cannot open pipe\n");
exit(-1);
}

while (!full) {
if (write(file[1], "p", 1) > 0) {
result++;
} else {
full = 1;
}
}

printf("answer: %d\n", result);
if (close(file[0]) < 0) {
printf("error: Cannot close reading side of pipe\n");
exit(-1);
}
if (close(file[1]) < 0) {
printf("error: Cannot close writing side of pipe\n");
exit(-1);
}

return 0;
}
