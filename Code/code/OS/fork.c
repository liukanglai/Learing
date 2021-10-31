#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 5

int nums[SIZE] = {0, 1, 2, 3, 4};

int main(void) {
  int i;
  pid_t pid;
  // int count = 0;
  // pid = 1;

  pid = fork();

  printf("%d\n", pid);

  if (pid < 0) {
    fprintf(stderr, "Fork Filed");
    return 1;
  }

  else if (pid == 0) {
    // execlp("bin/ls", "ls", NULL);
    // count++;
    // printf("%d, %d\n", pid, count);
    for (i = 0; i < SIZE; i++) {
      *(nums + i) *= -i;
      printf("child: %d %p ", nums[i], nums + i);
    }
  }

  else {
    printf("Go go Child\n");
    wait(NULL);
    printf("\nChild Complete\n");
    for (i = 0; i < SIZE; i++) {
      printf("father: %d %p ", nums[i], nums + i);
    }
    // count++;
    // printf("%d, %d\n", pid, count);
  }
  for (i = 0; i < SIZE; i++) {
    printf("child: %d %p ", nums[i], nums + i);
  }

  // printf("%d, %d\n", pid, count);
  // printf("h");
  return 0;
}
