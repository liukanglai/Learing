/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

/*
 * Reference:
 * http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/shell.c
 */

#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h> // for strcmp
//#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int parse(char *line, char **args) {
  while (*line != '\0') { /* if not the end of line ....... */
    while (*line == ' ' || *line == '\t' || *line == '\n')
      *(line++) = '\0'; /* replace white spaces with 0    */
    *(args++) = line;   /* save the argument position     */
    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
      line++; /* skip the argument until ...    */
  }
  if (*(args - 1) != NULL &&
      strcmp(*(args - 1), "&") == 0) { // 引用指针..............
    *(args - 1) = NULL;
    return 1;
  }
  *args = NULL; /* mark the end of argument list  */
  return 0;
}

void execute(char **args, int flag) {
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) { // fork a child process           */
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  } else if (pid == 0) {           // for the child process:         */
    if (execvp(*args, args) < 0) { // execute the command  */
      printf("*** ERROR: exec failed\n");
      exit(1); // 发现这一行至关重要？
    }
    exit(1);
  } else { // for the parent:      */
    if (!flag) {
      wait(NULL); // 有问题，待解决......
                  /*while (wait(&status) != pid) // wait for completion*/
                  /*;*/
                  /*}*/
    }
  }
}

int head = 0, end = -1, full = 0;

void inqueue(char history[][128], char *line) {
  // 易错
  /*if (++end >= 10) {*/
  /*end %= 10;*/
  /*head++;*/
  /*head %= 10;*/
  /*}*/
  if (++end >= 10) {
    end %= 10;
    full = 1;
  }
  strcpy(history[end], line);
  if (full) {
    head++;
    head %= 10;
  }
}

void outqueue(char history[10]) {}

void save_history(char history[][128], char *line, int *count) {
  /*
  for (int i = 9; i >= 0; i--) {
    strcpy(history[i], history[i - 1]);
  }
  */
  // strcpy(history[0], line);
  inqueue(history, line);
  if (*count < 10)
    (*count)++;
}

int show_history(char history[][128], int number, int count) {
  if (count == 0) {
    printf("No commands in history.\n");
    return 0;
  }
  if (number == -1) {
    for (int i = 0; i < count; i++) {
      printf("%d %s\n", count - i, history[(end - i + 10) % 10]);
    }
  } else {
    if (number > count || number <= 0) {
      printf("No such command in history.\n");
      return 0;
    }
    printf("%d %s\n", number, history[(head + number - 1) % 10]);
    // printf("%d %d\n", head, end);
  }
  return 1;
}

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void) {
  char *
      args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
  char history[10][128];

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 128; j++) {
      history[i][j] = '\0';
    }
  }

  int should_run = 1;
  int count = 0;

  char line[128]; // the input line

  while (should_run) {
    int flag = 0;
    printf("osh>");
    fflush(stdout);
    gets(line); // no '\n', have '\0'
    // fgets(line, MAX_LINE, stdin); //
    // 只有指针 NULL，才不运行，有一 '\0' 做参数，因为回车.
    //
    /*printf("%p0 ", line);*/
    /*fputs(line, stdout);*/

    if (strcmp(line, "exit") == 0) {
      should_run = 0;
      continue;
    }

    if (strcmp(line, "history") == 0) {
      show_history(history, -1, count);
      continue;
    }

    // 将 ! 后运行的历史命令也存入了.
    if (strcmp(line, "!!") == 0) {
      if (1 == show_history(history, count, count))
        strcpy(line, history[(head + count - 1) % 10]);
      // about !10. input !2.... wrong
    } else if (line[0] == '!') {
      int number = atoi(line + 1);
      if (1 == show_history(history, number, count))
        strcpy(line, history[(head + number - 1) % 10]);
    }

    /**
     * After reading user input, the steps are:
     * (1) fork a child process
     * (2) the child process will invoke execvp()
     * (3) if command includes &, parent and child will run concurrently
     */

    save_history(history, line, &count);
    flag = parse(line, args);
    execute(args, flag);
    // clear args, no need, it's a pointer
  }
}
