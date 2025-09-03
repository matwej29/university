

#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/wait.h> // wait()
#include <unistd.h>   // getpid(), getppid(),fork()

#define READ 0
#define WRITE 1

void child_a(int fd[]) {
  printf(" CHILD1 <%ld> Started!\n", (long)getpid());
  dup2(fd[1], 1);
  close(fd[0]); // чтение
  execlp("ls", "ls", "-F", "-1", NULL);
  printf(" CHILD1 <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

void child_b(int fd[]) {
  printf(" CHILD2 <%ld> Started!\n", (long)getpid());
  dup2(fd[0], 0);
  close(fd[1]); // запись
  execlp("nl", "nl", NULL);
  printf(" CHILD2 <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

int main(void) {
  int fd[2];
  pipe(fd);

  pid_t pid1;

  switch (pid1 = fork()) {
  case -1:
    exit(EXIT_FAILURE);
  case 0:
    child_a(fd);
    break;
  default:
    // процесс-родитель

    break;
  }

  pid_t pid2;
  switch (pid2 = fork()) {
  case -1:
    exit(EXIT_FAILURE);
  case 0:
    child_b(fd);
    break;
  default:
    // процесс-родитель
    close(fd[READ]);
    close(fd[WRITE]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    printf("PARENT: Done!\n");
    exit(EXIT_SUCCESS);
    break;
  }
}
