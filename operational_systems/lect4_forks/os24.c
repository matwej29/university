#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sighandler(int signum) {
  printf("\nCaught signal %d\n", signum);
  printf("Waiting a name ...\n");
  alarm(5);
}

int main() {
  char s[80];
  signal(SIGALRM, sighandler);
  alarm(5);
  printf("Input a name ...\n");
  for (;;) {
    printf("Name: ");
    if (fgets(s, sizeof(s), stdin) != NULL)
      break;
  }
  printf("OK! \n");
  return 0;
}
