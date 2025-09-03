#include <pthread.h>   /* pthread_...() */
#include <semaphore.h> /* sem_...() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* abort(), [s]rand() */
#include <unistd.h>    /* sleep() */
#define LOOPS 5
#define NTHREADS 3
#define MAX_SLEEP_TIME 3

sem_t *semA;
sem_t *semB;

void *threadA(void *param __attribute__((unused))) {
  for (int i = 0; i < LOOPS; i++) {
    sem_wait(semA);
    printf("A%d\n", i);
    sleep(rand() % MAX_SLEEP_TIME); // Симулируем работу
    sem_post(semB);
  }
  pthread_exit(0);
}

/* TODO: Заставьте два потока выполнять свои итерации в синхронном режиме. */
void *threadB(void *param __attribute__((unused))) {
  for (int i = 0; i < LOOPS; i++) {
    sem_wait(semB); // Wait for B turn
    printf("B%d\n", i);
    sleep(rand() % MAX_SLEEP_TIME);
    sem_post(semA); // Signal A
  }
  pthread_exit(0);
}

int main() {
  pthread_t tidA, tidB;
  srand(time(NULL));

  semA = sem_open("/semA", O_CREAT, 0644, 1);
  semB = sem_open("/semB", O_CREAT, 0644, 1);

  if (pthread_create(&tidA, NULL, threadA, NULL) ||
      pthread_create(&tidB, NULL, threadB, NULL)) {
    perror("pthread_create");
    abort();
  }
  if (pthread_join(tidA, NULL) != 0 || pthread_join(tidB, NULL) != 0) {
    perror("pthread_join");
    abort();
  }

  sem_unlink("/semA");
  sem_unlink("/semB");
  return 0;
}
