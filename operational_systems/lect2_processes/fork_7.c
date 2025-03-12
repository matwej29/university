#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/wait.h> // wait()
#include <unistd.h>   // getpid(), getppid(),fork()
void child() {
  printf(" CHILD <%ld> мой PPID = %ld.\n", (long)getpid(), (long)getppid());
  printf(" CHILD <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}
void parent(pid_t pid) {
  printf("PARENT <%ld> Порожден ребенок с PID = %ld.\n", (long)getpid(),
         (long)pid);

  printf("PARENT <%ld> Нажмите клавишу, чтобы получить зомби!\n",
         (long)getpid());
  getchar();
  pid = wait(NULL);
  printf("PARENT <%ld> Ребенок-зомби с PID = %ld !\n", (long)getpid(),
         (long)pid);
  printf("PARENT <%ld> Нажмите клавишу для завершения!\n", (long)getpid());
  getchar();
  printf("PARENT <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}
int main(void) {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  pid_t pid;
  switch (pid = fork()) {
  case -1: // При ошибке fork() возвращает -1.
    perror("fork failed");
    exit(EXIT_FAILURE);
  case 0: // При успехе fork() возвращает 0 в дочерней функции.
    child();
  default: // При успехе fork() возвращает pid дочернего объекта.
    parent(pid);
  }
}
