#include <pthread.h> // pthread_attr_init(), pthread_create(), pthread_join ()
#include <stdio.h>   // printf()
#include <stdlib.h>  // exit(), EXIT_FAILURE
#include <unistd.h>  // sleep()
#define NUM_OF_THREADS 2
void *worker(void *arg) {
  int id = *(int *)arg;
  printf("PID = <%ld> Нить %d говорит: Hello!\n", (long)getpid(), id);
  for (int i = 2; i > 0; i--) {
    printf("PID = <%ld> Нить %d говорит: Мне осталось жить %d секунд.\n",
           (long)getpid(), id, i);
    sleep(1);
  }
  return NULL;
}
int main(int argc, char *argv[]) {
  pthread_t tid[NUM_OF_THREADS];
  int targ[NUM_OF_THREADS];
  pthread_attr_t attr;      // Атрибуты
  pthread_attr_init(&attr); // атрибуты по умолчанию
  printf("PID = <%ld> ==> Давайте создадим несколько потоков.\n",
         (long)getpid());
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    targ[i] = i;
    pthread_create(&tid[i], &attr, worker, &targ[i]);
  }
  switch (fork()) {
  case -1:
    perror("Forked failed.");
    exit(EXIT_FAILURE);
  case 0:
    printf("PID = <%ld> ==> Child after fork()!\n", (long)getpid());
    break;
  default:
    printf("PID = <%ld> ==> Parent after fork()!\n", (long)getpid());
  }
  /* Ждем завершения всех потоков.*/
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    if (pthread_join(tid[i], NULL) == 0)
      printf("PID = <%ld> ==> Дождались завершения потока %d.\n",
             (long)getpid(), i);
    else {
      printf("PID = <%ld> ==> No thread to join.\n", (long)getpid());
    }
  }
}
