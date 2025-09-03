#include <stdio.h>
#include <pthread.h>
// Глобальная переменная для блокировки
volatile int _lock = 0;
// Функция для захвата блокировки
void lock(volatile int *lock) {
	while (__sync_val_compare_and_swap (lock, 0, 1) == 1) {
    }
}
// Функция для освобождения блокировки
void unlock(volatile int *lock) {
	*lock = 0;// Освобождаем ресурс
}
// Общий ресурс
int counter = 0;
// Функция, которую выполняют потоки
void* thread_function(void* arg) {
    for (int i = 0; i < 1000000000; i++) {
        lock(&_lock);   // Захватываем блокировку
        counter++;      // Работаем с общим ресурсом
        unlock(&_lock); // Освобождаем блокировку
    }
    return NULL;
}
int main() {
    pthread_t thread1, thread2;
    // Создаем два потока
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);
    // Ждем завершения потоков
    pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
    // Выводим результат
    printf("counter = %d\n", counter);
    return 0;
}