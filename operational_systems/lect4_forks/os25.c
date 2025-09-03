#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void sighandler(int signum) {
  printf("Caught signal %d: waiting...\n", signum);
  signal(SIGALRM, sighandler);
}

int main(int argc, char **argv) {
  char s[80];
  int pid;
  signal(SIGALRM, sighandler);
  if (pid = fork()) { // parent
    for (;;) {
      sleep(5);
      kill(pid, SIGALRM); // SIGALRM to child
    }
  } else { // child
    printf("Input a name ...\n");
    for (;;) {
      printf("Name: ");
      if (fgets(s, sizeof(s), stdin) != NULL)
        break;
    }
    printf("OK! \n");
    kill(getppid(), SIGKILL); // SIGKILL to parent
  }
  return 0;
}
