// Time:2021-12-08 21:40:10
#include <pthread.h>
#include <stdbool.h> // use bool
#include <stdio.h>
#include <stdlib.h> // malloc

/* queue status and conditional variable*/
typedef struct staconv {
  pthread_mutex_t mutex;
  pthread_cond_t cond; /*用于阻塞和唤醒线程池中线程*/
  int status; /*表示任务队列状态:false 为无任务;true 为有任务*/
} staconv;

typedef struct task {
  struct task *next;           /* 指向下一任务 */
  void (*function)(void *arg); // 函数指针
  void *arg;
} task;

typedef struct taskqueue {
  pthread_mutex_t mutex; /* 用于互斥读写任务队列 */
  task *front;           // 指向队首
  task *rear;            // 指向队尾
  staconv *has_jobs;     // 根据状态,阻塞线程
  int len;               // 队列中任务个数
} taskqueue;

typedef struct thread {
  int id;
  pthread_t pthread;
  struct threadpool *pool;
} thread;

typedef struct threadpool {
  thread **threads;             // 线程指针数组
  volatile int num_threads;     /* 线程池中线程数量 */
  volatile int num_working;     /* 目前正在工作的线程个数 */
  pthread_mutex_t thcount_lock; /* 线程池锁用于修改上面两个变量 */
  pthread_cond_t threads_all_idle; /* 用于销毁线程的条件变量 */
  taskqueue queue;                 /* 任务队列 */
  volatile bool is_alive;          /* 表示线程池是否还存活 */
} threadpool;

struct threadpool *initTheadPool(int num_threads) {
  threadpool *pool; //创建线程池空间
  pool = (threadpool *)malloc(sizeof(struct threadpool));
  pool->num_threads = 0;

  pool->num_working = 0;
  //初始化互斥量和条件变量
  pthread_mutex_init(&(thpool_p->thcount_lock), NULL);
  pthread_cond_init(&thpool_p->threads_all_idle, NULL);
  //初始化任务队列
  //****需实现*****
  init_taskqueue(&pool->queue);
  pool->threads = //创建线程数组
      (struct thread **)malloc(num_threads * sizeof(struct * thread));
  for (int i = 0; i < num_threads; ++i) { // i 为线程 id
    create_thread(pool, pool->thread[i], i);
  }
  //等等所有的线程创建完毕,在每个线程运行函数中将进行 pool->num_threads++ 操作
  //因此,此处为忙等待,直到所有的线程创建完毕,并⻢上运行阻塞代码时才返回。
  while (pool->num_threads != num_threads) {
  }
  return pool;
}

/*向线程池中添加任务*/
void addTask2ThreadPool(threadpool *pool, task *curtask) {
  //将任务加入队列
  //****需实现*****
  push_taskqueue(&pool->queue, curtask);
}

/*等待当前任务全部运行完*/
void waitThreadPool(threadpool *pool) {
  pthread_mutex_lock(&pool->thcount_lock);
  while (pool->jobqueue.len || pool->num_threads_working) {
    pthread_cond_wait(&pool->threads_all_idle, &pool->thcount_lock);
  }
  pthread_mutex_unlock(&thpool_p->thcount_lock);
}

/*销毁线程池*/
void destoryThreadPool(threadpool *pool) {
  //如果当前任务队列中有任务,需等待任务队列为空,并且运行线程执行完任务后
  ............
      //销毁任务队列
      //****需实现*****
      destory_taskqueue(&pool->queue);
  //销毁线程指针数组,并释放所有为线程池分配的内存
  ............
}
/*获得当前线程池中正在运行线程的数量*/
int getNumofThreadWorking(threadpool *pool) { return pool->num_working; }
/*创建线程*/
int create_thread(struct threadpool *pool, struct thread **pthread, int id) {
  //为 thread 分配内存空间
  *pthread = (struct thread *)malloc(sizeof(struct thread));
  if (pthread == NULL) {
    error("creat_thread(): Could not allocate memory for thread\n");
    return -1;
  }
  //设置这个 thread 的属性
  //
  (*pthread)->pool = pool;
  (*pthread)->id = id;
  //创建线程
  pthread_create(&(*pthread)->pthread, NULL, (void *)thread_do, (*pthread));
  pthread_detach((*pthread)->pthread);
  return 0;
}
/*线程运行的逻辑函数*/
void *thread_do(struct thread *pthread) {
  /* 设置线程名字 */
  char thread_name[128] = {0};
  sprintf(thread_name, "thread-pool-%d", thread_p->id);
  prctl(PR_SET_NAME, thread_name);
  /* 获得线程池*/
  threadpool *pool = pthread->pool;
  /* 在线程池初始化时,用于已经创建线程的计数,执行 pool->num_threads++ */
  ............
      /*线程一直循环往复运行,直到 pool->is_alive 变为 false*/
      while (pool->is_alive) {
    /*如果任务队列中还要任务,则继续运行,否则阻塞*/
    ............ if (pool->is_alive) {
      /*执行到此位置,表明线程在工作,需要对工作线程数量进行计数*/
      // pool->num_working++
      ............
          /* 从任务队列的队首提取任务,并执行*/
          void (*func)(void *);
      void *arg;
      // take_taskqueue 从任务队列头部提取任务,并在队列中删除此任务
      //****需实现 take_taskqueue*****
      task *curtask = take_taskqueue(&pool->queue);
      if (curtask) {
        func = curtask->function;
        arg = curtask->arg;
        //执行任务
        func(arg);
        //释放任务
        free(curtask);
      }
      /*执行到此位置,表明线程已经将任务执行完成,需更改工作线程数量*/
      //此处还需注意,当工作线程数量为 0,表示任务全部完成,要让阻塞在
      // waitThreadPool 函
      数上的线程继续运行
      ............
    }
  }
  /*运行到此位置表明,线程将要退出,需更改当前线程池中的线程数量*/
  // pool->num_threads--
  ........return NULL;
}

int main(void) {}
