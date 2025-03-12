#include <stdio.h>  // puts(), printf(), perror(), getchar()
#include <stdlib.h> // exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h> // getpid(), getppid()

int main(void) {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  printf(" CHILD <%ld> мой PPID = %ld.\n", (long)getpid(), (long)getppid());

  printf(" CHILD <%ld> Нажмите любую клавишу, чтобы я прекратил работу!\n",
         (long)getpid());

  getchar();

  printf(" CHILD <%ld> Goodbye!\n", (long)getpid());

  exit(127);
}
