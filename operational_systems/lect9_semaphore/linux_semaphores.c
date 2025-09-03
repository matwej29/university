#include <stdio.h> // perror()
#include <stdlib.h> // malloc()
#include "linux_semaphores.h"
sem_t *s_init(unsigned int value) {
  sem_t *sem = malloc(sizeof(sem_t));
  if (sem_init(sem, 0, value) == -1) {
	  perror("Initializing new semaphore");
	  abort();
 	}
  sem_open("", O_CREAT, 0644, 0);
	return sem;
}
void s_wait(sem_t *sem) {
  if (sem_wait(sem) == -1) {
    perror("Wating on sempahore failed");
    abort();
  }
}
void s_signal(sem_t *sem) {
  if (sem_post(sem) == -1) {
    perror("Signaling on semaphore failed");
    abort();
  }
}
void s_destroy(sem_t *sem) {
  if (sem_destroy(sem) == -1) {
    perror("Destroying semaphore failed");
    abort();
  }
}
