#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> //access to the POSIX operating system API.

#define NUM 100
#define SEM_NAME "sem_example"
#define SHM_NAME "mmap_example"

int main(void) {

  int count = 0;
  sem_t *psem;

  //创建信号量,初始信号量为 1
  if ((psem = sem_open(SEM_NAME, O_CREAT, 0666, 1)) == SEM_FAILED) {
    perror("create semaphore error");
    exit(1);
  }
  int shm_fd;
  //创建共享内存对象
  if ((shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666)) < 0) {
    perror("create shared memory object error");
    exit(1);
  }
  /* 配置共享内存段大小*/
  ftruncate(shm_fd, sizeof(int));
  //将共享内存对象映射到进程
  void *memPtr =
      mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (memPtr == MAP_FAILED) {
    perror("create mmap error");
    exit(1);
  }
  //为此内存区域赋值
  *(int *)memPtr = count;
  //创建子进程
  pid_t pid = fork();
  if (pid == 0) // child process
  {
    for (int i = 0; i < NUM; ++i) {
      //信号量实现的临界区
      sem_wait(psem);
      printf("Child Process count value: %d\n", (*(int *)memPtr)++);
      sem_post(psem);
    }
  } else if (pid > 0) { // parent process
    for (int i = 0; i < NUM; ++i) {
      //信号量实现的临界区
      sem_wait(psem);
      printf("Parent Process count value: %d\n", (*(int *)memPtr)++);
      sem_post(psem);
    }
    // wait(NULL) ???难搞 1ms enough
    sleep(1);
    //卸载各种资源
    if (munmap(memPtr, sizeof(int)) == -1) {
      perror("unmap failed");
      exit(1);
    }
    if (close(shm_fd) == -1) {
      perror("close shm failed");
      exit(1);
    }
    if (shm_unlink(SHM_NAME) == -1) {
      perror("shm_unlink error ");
      exit(1);
    }
    if (sem_close(psem) == -1) {
      perror("close sem error");
      exit(1);
    }
    if (sem_unlink(SEM_NAME) == -1) {
      perror("sem_unlink error");
      exit(1);
    }
  } else {
    perror("create childProcess error");
    exit(1);
  }
  exit(0);
  // return 0;
}
