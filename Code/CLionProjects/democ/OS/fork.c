#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void print(int a) { printf("%d\n", a); }

#define SIZE 5

int nums[SIZE] = {0, 1, 2, 3, 4};

int main(void) {
  int i;
  pid_t pid;

  pid = fork();

  for (int i = 0; i < 5; i++) {

    if (pid < 0) {
      fprintf(stderr, "Fork Filed");
      return 1;
    }

    else if (pid == 0) {
      // execlp("bin/ls", "ls", NULL);
      // count++;
      // printf("%d, %d\n", pid, count);
      print(i);
    }

    else {
      printf("Go go Child\n");
      wait(NULL);
      printf("\nChild Complete\n");
      print(i);
    }
  }

  printf("End\n");

  return 0;
}
