#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#define NUM_OF_THREADS 4

void* hello(void* arg) {
  int i = *(int*) arg;
  printf("Thread %d says hello!\n", i);
  int* status = malloc(sizeof(int));
  *status = i*10;
  pthread_exit(status);
}

int main(int argc, char *argv[]) {
  /* Массив идентификаторов потоков*/
  pthread_t tid[NUM_OF_THREADS];
  int arg[NUM_OF_THREADS];
  /* Атрибуты (размер стека, информация о планировании) для потоков*/
  pthread_attr_t attr;
  /* Получение атрибутов по умолчанию для потоков. */
  pthread_attr_init(&attr);
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    arg[i] = i;
    pthread_create(&tid[i], &attr, hello, &arg[i]);
  }
  int *status_ptr[NUM_OF_THREADS];
  /* Ждем завершения всех потоков. */
  for (int i = 0; i < NUM_OF_THREADS; i++){
    if (pthread_join(tid[i], (void**)&(status_ptr[i])) != 0) {
      perror("pthread_join");
      exit(EXIT_FAILURE);
    }
    printf("Thread %d says goodbye with status = %d\n", i, *status_ptr[i]);
  }
}
