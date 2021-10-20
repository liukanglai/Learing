#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid;
  int count = 0;

  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork Filed");
    return 1;
  }

  else if (pid == 0) {
    execlp("bin/ls", "ls", NULL);
    count++;
    printf("%d, %d\n", pid, count);
  }

  else {
    wait(NULL);
    printf("Child Complete\n");
    count++;
    printf("%d, %d\n", pid, count);
  }

  printf("%d, %d\n", pid, count);
  return 0;
}
