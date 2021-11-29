#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define random_1(a, b) ((rand() % (b - a)) + a) //随机值将含a不含b
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int value = 0;
long double valid = 0, all = 999999; //初始化有效点为0,总点数为999999
long double pi = 0;
// void*  fun(void* arg) {

void fun() {

  // pthread_mutex_lock(&mutex);
  long double x, y; //定义坐标x,y

  int i = 1;

  srand((double)time(NULL)); //设置随机数种子，防止每次产生的随机数相同

  for (i = 1; i <= 999999; i++) {

    //获取x,y的随机值,0<=x<100000,0<=y<100000
    x = random_1(0, 100000);
    y = random_1(0, 100000);
    if (x * x + y * y <= 10000000000)
#pragma omp crirical // OpenMP并行编排支持
    {

      valid++;
    }
  }
  // pthread_mutex_unlock(&mutex);
  //  return (void*)0;
}
//主函数
int main() {

  clock_t start, end;
  start = clock(); //读取程序开始运行的时间

  fun();
  end = clock(); //记录结束时间
  pi = valid / 999999 * 4;
  printf("valid的值为： %Lf\n", valid);
  printf("总的点数的值为999999\n");
  printf("π的值为： %Lf\n", pi);
  double seconds = (double)(end - start) / CLOCKS_PER_SEC * 1000;
  printf("Use time is: %.3fms\n", seconds);
  //进程锁销毁
  pthread_mutex_destroy(&mutex);
  return 0;
}
