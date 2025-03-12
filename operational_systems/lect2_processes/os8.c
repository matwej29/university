#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid = fork();
  if (pid == 0) {
    int child = getpid();
    printf("Child (%d): parent %d, group %d\n", getpid(), getppid(),
           getpgid(child));
    sleep(8);
    printf("Child(%d): parent %d, group %d\n", getpid(), getppid(),
           getpgid(child));
    sleep(4);
    printf("Child(%d): parent %d, group %d\n", getpid(), getppid(),
    getpgid(child));
  } else {
    int parent = getpid();
    printf("Parent(%d): parent %d, group %d\n", getpid(), getppid(),
           getpgid(parent));
    sleep(4);
    int zero = 0;
    int u = 5 / zero;
  }
  return 0;
}
