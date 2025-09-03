#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const char *tempfile = "abc2";
void sighandler(int signum) {
  printf("Caught signal %d...\n", signum);
  unlink(tempfile);
  exit(0);
}

int main() {
  signal(SIGINT, sighandler);
  creat(tempfile, 0666);
  sleep(30);
  unlink(tempfile);
  return (0);
}
