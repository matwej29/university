#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid = fork();
  if (pid == 0) {
    execlp("ls", "ls", NULL);
    printf("this will only happen if exec fails\n");
  } else {
    wait(NULL);
    printf("we're done\n");
  }
  return 0;
}
