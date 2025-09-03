#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void zot(void) {
  unsigned long r = 0x333;
  unsigned long r1 = 0x444;
  unsigned long *i;

  printf("%p 0x%lx\n", &r, *(&r));
  printf("%p 0x%lx\n", (&r + 1), *(&r + 1));
  printf("%p 0x%lx\n", (&r + 2), *(&r + 2));
  printf("%p 0x%lx\n", (&r + 3), *(&r + 3));
  printf("%p 0x%lx\n", (&r + 4), *(&r + 4));
  printf("%p 0x%lx\n", (&r + 5), *(&r + 5));
  printf("%p 0x%lx\n", (&r + 6), *(&r + 6));
  printf("%p 0x%lx\n", (&r + 7), *(&r + 7));
  printf("%p 0x%lx\n", (&r + 8), *(&r + 8));
  printf("%p 0x%lx\n", (&r + 9), *(&r + 9));
  printf("%p 0x%lx\n", (&r + 10), *(&r + 10));
  printf("%p 0x%lx\n", (&r + 11), *(&r + 11));
  printf("---zot %p\n", (void *)i);
}

void foo(void) {
  unsigned long q = 0x222;
  printf("---foo %p \n", &q);
  zot();
back1:
  printf(" backl: %p\n", &&back1);
}

int main() {
  unsigned long pid = (unsigned long)getpid();
  unsigned long p = 0x111;
  foo();
back:
  printf(" p: %p\n", &p);
  printf(" pid: %p\n", &pid);
  printf(" back: %p\n", &&back);
  printf("process id: %lx\n", pid);
  printf("\n\n /proc/%lu/maps\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%lu/maps", pid);
  system(command);
  return 0;
}