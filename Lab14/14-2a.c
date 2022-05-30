#include <signal.h>
#include <stdio.h>

void myHandler(int nsig) {
  if(nsig == SIGINT) {
    printf("CTRL + Снажат");
  }
  else if(nsig == SIGQUIT) {
    printf("CTRL + 4 нажат");
  }
}

int main(void) {
  (void)signal(SIGINT, myHandler);
  (void)signal(SIGQUIT, myHandler);

  while(1);
  return 0;
}
