#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print(int a) { printf("%d\n", a); }

#define SIZE 5

int nums[SIZE] = {0, 1, 2, 3, 4};

int main(void) {
  int i;
  int a;

  for (i = 1;; i++) {
    scanf("%d", &a);
    pid_t pid = 1;
    if (pid == 1) {
      printf("%d ", i);
    }
    pid = fork();
    if (pid == 0) {
      printf("pid:%d Child", pid);
      exit(0);
    }
    if (pid > 0) {
      printf("father");
    }
    /*printf("%d", i);*/
  }

  /*for (int i = 0; i < 5; i++) {*/

  /*if (pid < 0) {*/
  /*fprintf(stderr, "Fork Filed");*/
  /*return 1;*/
  /*}*/

  /*else if (pid == 0) {*/
  /*// execlp("bin/ls", "ls", NULL);*/
  /*// count++;*/
  /*// printf("%d, %d\n", pid, count);*/
  /*print(i);*/
  /*}*/

  /*else {*/
  /*printf("Go go Child\n");*/
  /*wait(NULL);*/
  /*printf("\nChild Complete\n");*/
  /*print(i);*/
  /*}*/
  /*}*/

  /*printf("End\n");*/

  return 0;
}
