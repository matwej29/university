#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_pid_t.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CNT 10
int target_pid, cnt;
int fd[2];
int status;
pid_t my_pid;

bool am_i_child = false;

void SigHndlr(int s) {
  // в обработчике сигнала происходит и чтение и запись
  signal(SIGUSR1, SigHndlr);
  if (cnt < MAX_CNT) {
    read(fd[0], &cnt, sizeof(int));
    printf("%d \n", cnt);
    cnt++;
    write(fd[1], &cnt, sizeof(int));
    // посылаем сигнал второму, пора читать из канала
    kill(target_pid, SIGUSR1);

    if (am_i_child) {
      printf("child %d sent SIGUSR1 to parent %d\n", my_pid, target_pid);
    } else {
      printf("parent %d sent SIGUSR1 to child %d\n", my_pid, target_pid);
    }
  } else if (target_pid == getppid()) {
    // условие окончания игры проверяется потомком
    printf("Child is going to be terminated\n");
    close(fd[1]);
    close(fd[0]);
    // завершается потомок
    exit(0);
  } else
    kill(target_pid, SIGUSR1);
}

int main(int argc, char **argv) {
  pipe(fd);
  signal(SIGUSR1, SigHndlr); // обработчик сигналов для обоих процессов
  cnt = 0;
  my_pid = getpid();
  if (target_pid = fork()) {
    printf("parent pid is %d", my_pid);
    wait(&status); // родитель ждет завершение потомка
    printf("Parent is going to be terminated\n");
    close(fd[1]);
    close(fd[0]);
    return 0;
  } else {
    target_pid = getppid(); // pid родителя
    if (my_pid == target_pid) {
      am_i_child = true;
    }
    my_pid = getpid();
    printf("child pid is %d", my_pid);
    write(fd[1], &cnt, sizeof(int));
    kill(target_pid, SIGUSR1);
    for (;;)
      ;
  }
}
