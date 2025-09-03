#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sighandler(int signum) {
  printf("\nCaught signal %d, coming out...\n", signum);
  exit(1);
}

int main() {
  signal(SIGINT, sighandler);
  while (1)
    ;
  return (0);
}
