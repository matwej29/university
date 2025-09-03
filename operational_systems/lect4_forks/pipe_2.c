#include <stdio.h>  // puts(), printf(), perror(), getchar()
#include <stdlib.h> // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>
#include <sys/wait.h> // wait()
#include <unistd.h>   // getpid(), getppid(),fork()
int fd[2];
void child() {
  char *s = "Hello from child!";
  printf(" CHILD <%ld> Мой родитель <%ld>\n", (long)getpid(), (long)getppid());
  close(fd[0]); // закрываем ненужный дескриптор
  write(fd[1], s, strlen(s) + 1);
  close(fd[1]);
  printf(" CHILD <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

void parent(pid_t pid) {
  printf("PARENT <%ld> Мой потомок <%ld>\n", (long)getpid(), (long)pid);
  char buf[80];
  close(fd[1]); // закрываем ненужный дескриптор
  read(fd[0], buf, 18);
  printf("PARENT <%ld> %s\n", (long)getpid(), buf);
  close(fd[0]);
  exit(EXIT_SUCCESS);
}
int main(void) {
  pid_t pid;
  pipe(fd); // организован канал
  switch (pid = fork()) {
  case -1:
    perror("fork failed");
    exit(EXIT_FAILURE);
  case 0: // процесс-потомок
    child();
  default: // процесс-родитель
    wait(NULL);
    parent(pid);
  }
}
