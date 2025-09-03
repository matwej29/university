#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int sum;
void *runner(void *param);
int main(int argc, char *argv[]) {
  pthread_t tid;       /* идентификатор потока */
  pthread_attr_t attr; /*набор атрибутов для потока */
  if (argc != 2) {
    fprintf(stderr, "usage: a.out <int value>\n");
    return -1;
  }
  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, runner, argv[1]);
  pthread_join(tid, NULL);
  printf("sum = %d\n", sum);
}
/* Поток начнет управление в этой функции*/
void *runner(void *param) {
  int i, upper = atoi(param);
  sum = 0;
  if (upper > 0) {
    for (i = 1; i <= upper; i++)
      sum += i;
  }
  pthread_exit(0);
}
