#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
foo:
  printf("the code: %p\n", &&foo);
  fgetc(stdin);

  int pid = getpid();
  unsigned long p = 0x1;
  printf("p (0x%lx): %p \n", p, &p);
  printf("process id: %d\n", pid);
  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);
  return 0;
}