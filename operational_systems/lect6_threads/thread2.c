#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_THREADS 4
void *hello(void *arg) {
  int i = *(int *)arg;
  printf("  thread %d - hello\n", i);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t tid[NUM_OF_THREADS]; // ID потоков
  int arg[NUM_OF_THREADS];       // данные для hello()
  pthread_attr_t attr;           // атрибуты потока
  pthread_attr_init(&attr);      // Получение атрибутов по умолчанию
  printf("main() - before creaing new threads\n");
  /* Создаем новые потоки (выполняют hello(). */
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    arg[i] = i;
    pthread_create(&tid[i], &attr, hello, &arg[i]);
  }
  /* Ждем завершения всех потоков. */
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    if (pthread_join(tid[i], NULL) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
    printf("main() - thread %d terminated\n", i);
  }
  printf("main() - all new threads terminated\n");
  exit(EXIT_SUCCESS);
}
