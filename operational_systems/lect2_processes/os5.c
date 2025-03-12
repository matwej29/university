#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("Щербаков Матвей Андреевич 36 группа\n");
  int pid;
  int x = 123;
  pid = fork();
  if (pid == 0) {
    printf("child: x is %d and address is 0x%p\n", x, &x);
    x = 42;
    sleep(1);
    printf("child: x is %d and address is 0x%p\n", x, &x);
  } else {
    printf(" parent: x is %d and address is 0x%p\n", x, &x);
    x = 13;
    sleep(1);
    printf("parent: x is %d and address is 0x%p\n", x, &x);
    wait(NULL);
  }
  return 0;
}
