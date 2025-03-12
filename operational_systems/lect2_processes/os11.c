#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid = fork();
  if (pid == 0) {
    int fd = open("f1.txt", O_RDWR | O_CREAT);
    dup2(fd, 1);
    close(fd);
    execl("myprint", "myprint", NULL);
    printf("this will only happen if exec fails\n");
  } else {
    wait(NULL);
    printf("we're done\n");
  }
  return 0;
}
