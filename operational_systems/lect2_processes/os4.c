#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid = fork();
  if (pid == 0) {
    printf("I am a child %d\n", getpid());
    sleep(10);
    printf("end sleep %d\n", getpid());
    return (42);
  } else {
    sleep(20);
    printf("I am %d. My child is called %d\n", getpid(), pid);
    int res;
    wait(&res);
    if (WIFEXITED(res))
      printf("My child has terminated: %d\n", WEXITSTATUS(res));
    printf("and again %d sleep\n", getpid());
    sleep(10);
  }
  printf("Finish %d\n", getpid());
  return 0;
}
