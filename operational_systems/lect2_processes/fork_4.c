#include <stdio.h>    // puts(), printf(), perror(), getchar()
#include <stdlib.h>   // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <sys/wait.h> // wait()
#include <unistd.h>   // getpid(), getppid(),fork()

void child() {
  printf(" CHILD <%ld> Мой PID - <%ld>, а мой родитель получил PID <%ld>.\n",
         (long)getpid(), (long)getpid(), (long)getppid());
  printf(" CHILD <%ld> Goodbye!\n", (long)getpid());
  sleep(50);
  exit(EXIT_SUCCESS);
}

void parent(pid_t pid) {
  printf("PARENT <%ld> Мой PID равен <%ld>, и я породил ребенка с PID <%ld>.\n",
         (long)getpid(), (long)getpid(), (long)pid);
  printf("PARENT <%ld> Goodbye!\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

int main(void) {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  pid_t pid;

  switch (pid = fork()) {
  case -1:
    // При ошибке fork() возвращается -1.
    perror("fork failed");
    exit(EXIT_FAILURE);
  case 0:
    // В случае успеха fork() возвращает 0 в дочернем файле.
    child();
  default:
    // В случае успеха fork() возвращает pid дочерней программы.
    parent(pid);
  }
}
