// https://www.alltestanswers.com/operating-systems-fundamentals-assignment-3/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <pthread.h>
#include <sys/time.h>

int threads = 0;
int total_point = 0;
int point_of_thread = 0; // allocation of every thread
int point_in_circle = 0;
pthread_mutex_t mutex; // 互斥锁

void *count(void *point_of_thread) {
  int point = *((int *)point_of_thread);

  /* initialize random numbers */
  int tem_point_in_circle = 0;
  for (int i = 0; i < point; i++) { // create points
    double X = (double)rand() / RAND_MAX;
    double Y = (double)rand() / RAND_MAX;

    if (((X * X) + (Y * Y)) <= 1) {
      tem_point_in_circle++; // 不在这加锁
    }
  }
  pthread_mutex_lock(&mutex);
  point_in_circle += tem_point_in_circle; // 用了 tem 做中介，很好的想法
  pthread_mutex_unlock(&mutex);

  pthread_exit(0);
  /*return NULL;*/
}

int main() {
  srand(time(NULL));

  printf("Please enter a positive number for the amount of points you would "
         "like to generate: ");
  scanf("%d", &total_point);
  printf("Please enter the threads you want: ");
  scanf("%d", &threads);

  pthread_t thread[threads];
  pthread_mutex_init(&mutex, NULL);

  struct timeval start, end;
  double timeuse;
  gettimeofday(&start, NULL);
  point_of_thread = total_point / threads;
  for (int i = 0; i < threads; i++) { // create threads
    pthread_create(&thread[i], NULL, &count, &point_of_thread);
  }
  for (int i = 0; i < threads; i++) { // join threads
    pthread_join(thread[i], NULL);
  }
  gettimeofday(&end, NULL);

  double pi = (4.0 * point_in_circle) / total_point;
  printf("The approximate value of pi for the desired amount of points (%d) "
         "is: %f \n",
         total_point, pi);
  timeuse = (end.tv_sec - start.tv_sec) +
            (double)(end.tv_usec - start.tv_usec) / 1000000.0;
  printf("use time: %f(s)", timeuse);
  return 0;
}
