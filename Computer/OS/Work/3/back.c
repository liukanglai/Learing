// Time:2021-12-13 14:52:29
// https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/
// 最开始一定是安全的，每一种分配都安全，那么最后一定 5
// 个线程运行完，所以程序成功的标志就是线程运行完，而不是一直生成随机数去
// request.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* these may be any values >= 0 */
#define NUMCUSTOMERS 5
#define NUMRESOURCES 3 // types
/* the available amount of each resource */
int available[NUMRESOURCES];
/*the maximum demand of each customer */
int maximum[NUMCUSTOMERS][NUMRESOURCES] = {
    {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
/* the amount currently allocated to each customer */
int allocation[NUMCUSTOMERS][NUMRESOURCES] = {
    {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
/* the remaining need of each customer */
int need[NUMCUSTOMERS][NUMRESOURCES];
int Request[NUMRESOURCES];

pthread_mutex_t mutex; // 互斥锁

int request_resources(int id, int request[3]) {
  int flag = 0;
  for (int i = 0; i < NUMRESOURCES; i++) {
    if (request[i] > available[i]) {
      flag = 1;
      break;
    }
  }
  if (flag)
    return -1;
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < NUMRESOURCES; i++) {
    available[i] -= request[i];
    allocation[id][i] += request[i];
    need[id][i] -= request[i];
  }
  pthread_mutex_unlock(&mutex);
  return 0;
}

int release_resources(int id, int release[3]) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < NUMRESOURCES; i++) {
    available[i] += release[i];
    allocation[id][i] -= release[i];
    need[id][i] += release[i];
  }
  pthread_mutex_unlock(&mutex);
  return 0;
}

void *customerRequest(void *cid) {
  int id = *(int *)cid; // customer id;
  int request[NUMRESOURCES];
  for (int k = 0;; k++) {
    // 限定 request < need，注意，need 可能为0...
    for (int i = 0; i < NUMRESOURCES; i++) {
      if (need[id][i] != 0)
        request[i] = (rand() % need[id][i]) + 1;
      else
        request[i] = 0;
    }

    if (request_resources(id, request) == 0) {
      int tem_available[NUMRESOURCES] = {available[0], available[1],
                                         available[2]};
      // check safe, need a temporary available array.
      int finish[NUMCUSTOMERS] = {0, 0, 0};
      /*printf("!!\n");*/
      for (int i = 0; i < NUMCUSTOMERS; i++) {
        if (finish[i] == 0) {
          int flag = 0;
          for (int j = 0; j < NUMRESOURCES; j++) {
            if (need[i][j] > tem_available[j]) {
              flag = 1;
              break;
            }
          }
          if (flag == 0) {
            for (int y = 0; y < NUMRESOURCES; y++)
              tem_available[y] += allocation[i][y];
            finish[i] = 1;
            i = 0; // 从头开始找 finish false.
          }
        }
      }
      // 所有 finish 都为 1, 就 safe
      int flag = 0;
      for (int i = 0; i < NUMCUSTOMERS; i++) {
        if (finish[i] == 0) {
          flag = 1;
          break;
        }
      }
      if (flag) // no safe.
        release_resources(id, request);
      else
        printf("success: id: %d, alloc: %d, %d, %d\n", id, request[0],
               request[1], request[2]);
    }
    // 判断 need 是否结束
    int flag = 0;
    for (int j = 0; j < NUMRESOURCES; j++) {
      if (need[id][j] != 0) {
        flag = 1;
        break;
      }
    }
    printf("id: %d, need: %d, %d, %d\n", id, need[id][0], need[id][1],
           need[id][2]);
    if (!flag)
      break;
  }
  printf("end: %d\n", id);
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  // 加一个 safe 判断。。。
  if (argc != 4) {
    printf("Please input 3 numbers of available resources!\n");
    return 1;
  }

  available[0] = atoi(argv[1]);
  available[1] = atoi(argv[2]);
  available[2] = atoi(argv[3]);

  pthread_t customerThreads[NUMCUSTOMERS];
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL));

  for (int i = 0; i < NUMCUSTOMERS; i++) {
    for (int j = 0; j < NUMRESOURCES; j++)
      need[i][j] = maximum[i][j] - allocation[i][j];
  }

  for (int i = 0; i < NUMCUSTOMERS; i++) {
    int *p = (int *)malloc(sizeof(int));
    *p = i;
    pthread_create(&customerThreads[i], NULL, &customerRequest, p);
  }
  for (int i = 0; i < NUMCUSTOMERS; i++) { // join threads
    pthread_join(customerThreads[i], NULL);
  }
  printf("Success!!!\n");

  /*printf("Following is the SAFE Sequence\n");*/
  /*for (int i = 0; i < NUMCUSTOMERS - 1; i++)*/
  /*printf(" P%d ->", ans[i]);*/
  /*printf(" P%d", ans[NUMCUSTOMERS - 1]);*/

  return 0;
}
