#include <pthread.h>   /* pthread_... */
#include <semaphore.h> /* sem_... */
#include <stdio.h>     /* printf(), fprintf */
#include <stdlib.h>    /* [s]rand() */
#include <unistd.h>    /* sleep() */

#define BUFFER_SIZE 5

#define PRODUCERS 2
#define CONSUMERS 4

#define ITERATIONS 20
#define PRODUCER_ITERATIONS (ITERATIONS / PRODUCERS)
#define CONSUMER_ITERATIONS (ITERATIONS / CONSUMERS)

    typedef struct {
      int value[BUFFER_SIZE];
      int next_in, next_out;
} buffer_t;

sem_t *empty;
sem_t *full;
sem_t *mutex;

buffer_t buffer;

pthread_t consumer_tid[CONSUMERS], producer_tid[PRODUCERS];

/* *
 * insert_item - thread safe(?) функция для вставки элементов в ограниченный
буфер  * @param item значение для вставки  * @return 0 в случае успеха -1 в
противном случае  */
int insert_item(int item, long int id) {
      /* TODO: Проверьте и подождите, не переполнен ли буфер. Обеспечьте
  эксклюзивность      * доступа к буферу и используйте существующий код для
  удаления элемента.      */
    sem_wait(empty);
      sem_wait(mutex);
       
    buffer.value[buffer.next_in] = item;
      buffer.next_in = (buffer.next_in + 1) % BUFFER_SIZE;
      printf("producer %ld: inserted %d\n", id, item);

      sem_post(mutex);
      sem_post(full);

      return 0;
}

/**
 * remove_item - thread safe(?) функция для удаления элементов в ограниченном
буфере  * @param item адрес переменной, в которую будет записано удаленное
значение  * @return 0 в случае успеха -1 в противном случае  */
int remove_item(int *item, long int id) {
      
    sem_wait(full);
      sem_wait(mutex);
       
    *item = buffer.value[buffer.next_out];
      buffer.value[buffer.next_out] = -1;
      buffer.next_out = (buffer.next_out + 1) % BUFFER_SIZE;
      printf("consumer %ld: removed %d\n", id, *item);

      sem_post(mutex);
      sem_post(empty);

      return 0;
}

/**
 * producer - будет повторять итерацию PRODUCER_ITERATION и вызывать
соответствующую  * функцию для вставки целого числа в ограниченный буфер  *
@param param целочисленный идентификатор производителя, используемый для
различения  * несколькими потоками производителей  * @return nothing  */
void *producer(void *param) {
      long int id = (long int)param;
      printf("producer started\n");
      int i = PRODUCER_ITERATIONS;
      while (i--) {
                sleep(rand() % 3);
                int item = rand() % 10000;
                if (insert_item(item, id))
                  fprintf(stderr, "Ошибка при вставке в буфер\n");
       
  }
      pthread_exit(0);
}

/**
 * consumer - будет повторять итерацию CONSUMER_ITERATION несколько раз и
вызывать соответствующую   * функцию для удаления целого числа из ограниченного
буфера  * @param param целочисленный идентификатор производителя, используемый
для различения  * несколькими потоками потребителей  * @return nothing  */
void *consumer(void *param) {
      int item;
      long int id = (long int)param;
      printf("consumer started\n");
      int i = CONSUMER_ITERATIONS;
      while (i--) {
                sleep(rand() % 3);
                if (remove_item(&item, id))
                  fprintf(stderr, "Ошибка при удалении из буфера\n");
          
  }
      pthread_exit(0);
}

int main() {
      long int i;
      
    empty = sem_open("/empty", 512, 0644, BUFFER_SIZE);
      full = sem_open("/full", 512, 0644, 0);
      mutex = sem_open("/mutex", 512, 0644, 1);
      

    srand(time(NULL));

      /* Создаем потоки потребителей */
    for (i = 0; i < CONSUMERS; i++)
      if (pthread_create(&consumer_tid[i], NULL, consumer, (void *)i) != 0) {
              perror("pthread_create");
              abort();
          
  }
      /* Создаем потоки производителей */
    for (i = 0; i < PRODUCERS; i++)
      if (pthread_create(&producer_tid[i], NULL, producer, (void *)i) != 0) {
              perror("pthread_create");
              abort();
          
  }

      /* Ожидаем завершения */
    for (i = 0; i < CONSUMERS; i++)
      if (pthread_join(consumer_tid[i], NULL) != 0) {
              perror("pthread_join");
              abort();
          
  }
      for (i = 0; i < PRODUCERS; i++)
      if (pthread_join(producer_tid[i], NULL) != 0) {
              perror("pthread_join");
              abort();
          
  }
        

    return 0;
}
