#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int volatile count;
void handler(int sig) {
  printf("signal %d ouch that hurt \n", sig);
  count++;
}

int main() {
  int pid = getpid();
  printf("ok, let's go, kill me (%d) if you can! \n", pid);
  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);
  while (count != 4) {
	pause();
  }
  printf("I've had enough!\n");
  return (0);
}
