#include "parser.h" // cmd_t, position_t, parse_commands()

#include <errno.h>
#include <fcntl.h> //fcntl(), F_GETFL
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

/**
 * Для простоты мы используем глобальный массив для хранения данных каждой
 * команды в конвейере команд.
 */
cmd_t commands[MAX_COMMANDS];

/**
 *  Отладочная распечатка массива команд.
 */
void print_commands(int n) {
  for (int i = 0; i < n; i++) {
    printf("==> commands[%d]\n", i);
    printf("  pos = %s\n", position_to_string(commands[i].pos));
    printf("  in  = %d\n", commands[i].in);
    printf("  out = %d\n", commands[i].out);

    print_argv(commands[i].argv);
  }
}

/**
 * Возвращает true, если файловый дескриптор fd открыт. В противном случае
 * возвращается false.
 */
int is_open(int fd) { return fcntl(fd, F_GETFL) != -1 || errno != EBADF; }

void fork_error() {
  perror("fork() failed)");
  exit(EXIT_FAILURE);
}

/**
 *  fork процесса для команды с индексом i в конвейере команд. Если необходимо,
 * создайте новый канал и обновите дескрипторы in и out для команды.
 */
void fork_cmd(int i) {
  pid_t pid;
  int fd[2];

  // Создаем канал только для команд, которые не являются single или last
  if (commands[i].pos != single && commands[i].pos != last) {
    if (pipe(fd) == -1) {
      perror("ERROR: shell.c; pipe() failed");
      exit(EXIT_FAILURE);
    }
  }

  switch (pid = fork()) {
  case -1:
    fork_error();
  case 0: // Дочерний процесс
    switch (commands[i].pos) {
    case single:
      break;
    case first:
      dup2(fd[WRITE], WRITE); // Перенаправляем вывод
      close(fd[READ]);        // Закрываем ненужный дескриптор
      break;
    case middle:
      dup2(commands[i].in, READ); // Устанавливаем чтение с дескриптора, который
                                  // сохранен в поле in
      dup2(fd[WRITE], WRITE);     // Перенаправляем вывод
      close(fd[READ]);            // Закрываем ненужный дескриптор
      break;
    case last:
      dup2(commands[i].in, READ); // Устанавливаем чтение с дескриптора, который
                                  // сохранен в поле in
      break;
    default:
      exit(EXIT_FAILURE);
    }

    // Выполняем команду
    execvp(commands[i].argv[0], commands[i].argv);
    fprintf(stderr, "shell: command not found: %s\n", commands[i].argv[0]);
    exit(EXIT_FAILURE);

  default: // Родительский процесс
    switch (commands[i].pos) {
    case single: // Для single и last мы не создаем канал, поэтому действий нет
    case last:
      break;
    case first:
    case middle:
      commands[i + 1].in = fd[READ]; // Сохраняем дескриптор для чтения в поле
                                     // in следующей команды
      close(fd[WRITE]);              // Закрываем дескриптор вывода
      break;
    default:
      exit(EXIT_FAILURE);
    }
  }
}

/**
 *  fork одного дочернего процесса для каждой команды в конвейере команд.
 *  child_pids должен быть массивом длины n, выделенным вызывающей стороной.
 */
void fork_commands(int n) {
  for (int i = 0; i < n; i++) {
    fork_cmd(i);
  }
}

/**
 * Считывает командную строку от пользователя и сохраняет строку в
 * буфере.
 */
void get_line(char *buffer, size_t size) {
  size_t a = getline(&buffer, &size, stdin);
  buffer[a - 1] = '\0';
}

/**
 * Заставьте родителей ждать все дочерние процессы.
 */
void wait_for_all_cmds(int n) {
  for (int i = 0; i < n; i++) {
    wait(NULL);
  }
}

int main() {
  int n;             // Количество команд в конвейере команд.
  size_t size = 128; // Максимальный размер командной строки.
  char line[size];   // Буфер для командной строки.

  while (true) {
    printf(" >>> ");

    get_line(line, size);

    n = parse_commands(line, commands);
    printf("shell: command found: %s\n", commands[0].argv[0]);
    print_commands(n);
    fork_commands(n);

    wait_for_all_cmds(n);
  }

  exit(EXIT_SUCCESS);
}
