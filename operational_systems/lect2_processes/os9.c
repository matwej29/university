#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid = fork();
  if (pid == 0) {
    int child = getpid();
    printf("Child (%d): session %d\n", getpid(), getsid(child));
  } else {
    int parent = getpid();
    wait(NULL);
    printf("Parent(%d): session %d\n", getpid(), getsid(parent));
  }
  return 0;
}
