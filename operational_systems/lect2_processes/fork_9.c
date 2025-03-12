#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/wait.h> // wait()
#include <unistd.h>   // getpid(), getppid(),fork()
void child() {
  char *const argv[] = {"./child_8", NULL};
  printf(" CHILD <%ld> Нажмите клавишу для вызова exec!\n", (long)getpid());
  fflush(stdout);
  getchar();
  execv(argv[0], argv);
  perror("execv");
  exit(EXIT_FAILURE);
}

void parent(pid_t pid) {
  int status;
  printf("PARENT <%ld> Создан ребенок с PID = %ld.\n", (long)getpid(),
         (long)pid);
  wait(&status);
  if (WIFEXITED(status)) {
    printf("PARENT <%ld> Ребенок с PID=%ld и статусом выхода=%d завершен.\n",
           (long)getpid(), (long)pid, WEXITSTATUS(status));
  }
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
  default: // При успехе fork() возвращает pid дочернего объекта родительскому.
    parent(pid);
  }
}
