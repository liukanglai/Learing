// Time:2021-12-08 21:40:10
#include <errno.h> // error
#include <pthread.h>
#include <stdbool.h> // use bool
#include <stdio.h>
#include <stdlib.h>    // malloc
#include <sys/prctl.h> //prctl

/* queue status and conditional variable*/
typedef struct staconv {
  pthread_mutex_t mutex;
  pthread_cond_t cond; /*用于阻塞和唤醒线程池中线程*/
  int status;
  /*表示任务队列状态:false 为无任务;true 为有任务*/ // 那 len 用来干嘛呢？
} staconv;

typedef struct task { // 怎样添加 web ？
  struct task *next;  /* 指向下一任务 */
  // void (*function)(void *arg);
  // 只是个函数声明，不是对类型的定义，感谢强大的代码报错工具（clang ale）
  // `https://stackoverflow.com/questions/21708566/void-functionvoid-argument-how-to-return-the-function-results`
  // void* vfunc(void *); is a function declaration, not a pointer object
  // definition. You probably want void *(*vfunc)(void *)
  void *(*function)(void *arg); // 函数指针
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

// volatile 变量，变化？
typedef struct threadpool {
  thread **threads;             // 线程指针数组
  volatile int num_threads;     /* 线程池中线程数量 */
  volatile int num_working;     /* 目前正在工作的线程个数 */
  pthread_mutex_t thcount_lock; /* 线程池锁用于修改上面两个变量 */
  pthread_cond_t threads_all_idle; /* 用于销毁线程的条件变量 */
  taskqueue queue;                 /* 任务队列 */
  // 这玩意不定义指针，真好，学到了
  volatile bool is_alive;
  /* 表示线程池是否还存活 */ // 有用吗？destoryThreadPool
                             // 时不全干掉了，防止其他导致线程的存活问题？
} threadpool;

void init_taskqueue(taskqueue *poolQueue) {
  // poolQueue = (taskqueue *)malloc(sizeof(taskqueue));
  //   这的问题？？？？为什么这一行去掉后才对........那是个结构体，不是指针。。。
  pthread_mutex_init(&(poolQueue->mutex), NULL);
  poolQueue->front = NULL;
  poolQueue->rear = NULL;

  poolQueue->has_jobs = (struct staconv *)malloc(sizeof(struct staconv));
  pthread_mutex_init(&(poolQueue->has_jobs->mutex), NULL);
  pthread_cond_init(&(poolQueue->has_jobs->cond), NULL);
  poolQueue->has_jobs->status = false;

  poolQueue->len = 0;
}

void destory_taskqueue(taskqueue *poolQueue) {
  pthread_mutex_destroy(&poolQueue->mutex);
  free(poolQueue->front);
  free(poolQueue->rear);
  pthread_mutex_destroy(&poolQueue->has_jobs->mutex);
  pthread_cond_destroy(&poolQueue->has_jobs->cond);
  free(poolQueue->has_jobs);
  // free(poolQueue);
}
void push_taskqueue(taskqueue *poolQueue, task *curtask) {
  pthread_mutex_lock(&poolQueue->mutex); // lock!
  if (poolQueue->front == NULL) {        // 分空队和非空讨论
    poolQueue->front = curtask;
  } else {
    poolQueue->rear->next = curtask;
  }
  poolQueue->rear = curtask;
  poolQueue->len++;
  pthread_mutex_unlock(&poolQueue->mutex);
}

task *take_taskqueue(
    taskqueue *poolQueue) { // take_taskqueue
                            // 从任务队列头部提取任务,并在队列中删除此任务
  task *tem_task;
  // pthread_mutex_lock(&poolQueue->mutex);
  if (poolQueue->front == NULL) //
    return NULL;
  pthread_mutex_lock(&poolQueue->mutex); // lock!
  tem_task = poolQueue->front;           // 可返回 NULL
  poolQueue->front = poolQueue->front->next;
  tem_task->next = NULL; //
  poolQueue->len--;
  pthread_mutex_unlock(&poolQueue->mutex);
  return tem_task;
}

/*线程运行的逻辑函数*/
void *thread_do(void *tem_pthread) {
  thread *pthread = (thread *)tem_pthread;
  /* 设置线程名字 */
  char thread_name[128] = {0};
  sprintf(thread_name, "thread-pool-%d", pthread->id);
  prctl(PR_SET_NAME, thread_name); // 重命名进程

  threadpool *pool = pthread->pool; /* 获得线程池*/
  /* 在线程池初始化时,用于已经创建线程的计数,执行 pool->num_threads++ */
  /*............*/
  pthread_mutex_lock(&pool->thcount_lock);
  pool->num_threads++;
  pthread_mutex_unlock(&pool->thcount_lock);

  /*线程一直循环往复运行,直到 pool->is_alive 变为 false*/
  while (pool->is_alive) {
    /*如果任务队列中还要任务,则继续运行,否则阻塞*/
    /*............ */
    pthread_mutex_lock(&(pool->queue.has_jobs->mutex));
    while (!pool->queue.has_jobs->status) { // vs queue.len ?
      // while (!pool->queue.len) { // 一个 signal 放多个线程。。。那么多个
      // take_taskqueue，里面空指针。。。而且时间大大延长
      pthread_cond_wait(&pool->queue.has_jobs->cond,
                        &pool->queue.has_jobs->mutex);
    }
    pool->queue.has_jobs->status = false; // 保证一次只要一个线程下来
    pthread_mutex_unlock(&(pool->queue.has_jobs->mutex));
    // printf("len:%d,id:%d\n", pool->queue.len, pthread->id);

    if (pool->is_alive) {
      /*执行到此位置,表明线程在工作,需要对工作线程数量进行计数*/
      /*............*/
      pthread_mutex_lock(&pool->thcount_lock);
      pool->num_working++;
      pthread_mutex_unlock(&pool->thcount_lock);
      // printf("working,id:%d,num_working:%d\n", pthread->id,
      // pool->num_working);

      /* 从任务队列的队首提取任务,并执行*/
      void *(*func)(void *);
      void *arg;
      // take_taskqueue 从任务队列头部提取任务,并在队列中删除此任务
      //****需实现 take_taskqueue*****
      task *curtask = take_taskqueue(&pool->queue);
      if (curtask) { // 有非空的判断
        func = curtask->function;
        arg = curtask->arg;
        //执行任务
        func(arg);
        //释放任务
        free(curtask);
      }
      /*执行到此位置,表明线程已经将任务执行完成,需更改工作线程数量*/
      //此处还需注意,当工作线程数量为 0,表示任务全部完成,要让阻塞在
      // waitThreadPool 函数上的线程继续运行
      /*............*/
      pthread_mutex_lock(&pool->thcount_lock);
      pool->num_working--;
      // if (pool->num_threads == 0) {
      // pthread_cond_signal(&pool->threads_all_idle);
      //}
      pthread_mutex_unlock(&pool->thcount_lock);
    }
  }
  /*运行到此位置表明,线程将要退出,需更改当前线程池中的线程数量*/
  /*........*/
  pthread_mutex_lock(&pool->thcount_lock);
  pool->num_threads--;
  pthread_mutex_unlock(&pool->thcount_lock);
  return NULL;
}

/*创建线程*/
int create_thread(threadpool *pool, thread **pthread, int id) {
  //为 thread 分配内存空间
  *pthread = (struct thread *)malloc(sizeof(struct thread));
  if (pthread == NULL) {
    // error("creat_thread(): Could not allocate memory for thread\n"); ???
    perror("creat_thread(): Could not allocate memory for thread\n");
    return -1;
  }
  //设置这个 thread 的属性
  (*pthread)->pool = pool; // pool 地址？
  (*pthread)->id = id;
  //创建线程
  pthread_create(&((*pthread)->pthread), NULL, thread_do, (void *)(*pthread));
  // printf("end?id:%d\n", id);
  pthread_detach(
      (*pthread)->pthread); // 设置为 detach 属性，一旦结束，自动释放，不用 join
  /*
   * pthread_attr_t attr;
   * pthread_attr_init(&attr);
   * pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   * 线程创建非常快，没有返回就结束，在创建线程里调用pthread_cond_wait;
   * 这为对条件变量的修改，需要加锁
   */
  return 0;
}

struct threadpool *initTheadPool(int num_threads) {
  threadpool *pool; //创建线程池空间
  pool = (threadpool *)malloc(sizeof(struct threadpool));
  pool->num_threads = 0;
  pool->num_working = 0;

  pool->is_alive = true; //?????????????????????
  //初始化互斥量和条件变量
  pthread_mutex_init(&(pool->thcount_lock), NULL);
  pthread_cond_init(&pool->threads_all_idle, NULL);

  //初始化任务队列
  init_taskqueue(&pool->queue); //****需实现*****
  pool->threads =               //创建线程数组
      (struct thread **)malloc(
          num_threads *
          sizeof(
              struct thread *)); // use pool->num_threads ..... 啊，终于找到错了
  for (int i = 0; i < num_threads; ++i) { // i 为线程 id
    create_thread(pool, &(pool->threads[i]), i);
    // printf("create: %d\n", i);
  }
  //等所有的线程创建完毕,在每个线程运行函数中将进行 pool->num_threads++ 操作
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
  pthread_mutex_lock(&pool->queue.has_jobs->mutex);
  pool->queue.has_jobs->status = true;
  if (pool->num_working != pool->num_threads)
    pthread_cond_signal(&pool->queue.has_jobs->cond); // 提出阻塞线程
  pthread_mutex_unlock(&pool->queue.has_jobs->mutex);
}

/*等待当前任务全部运行完*/
void waitThreadPool(threadpool *pool) {
  pthread_mutex_lock(&pool->thcount_lock);
  while (pool->queue.len || pool->num_working) { // cond 的机制不是很懂。。。
    pthread_cond_wait(&pool->threads_all_idle,
                      &pool->thcount_lock); // 这玩意会释放锁，嗯
  }
  pthread_mutex_unlock(&pool->thcount_lock);
}

/*销毁线程池*/
void destoryThreadPool(threadpool *pool) {
  //如果当前任务队列中有任务,需等待任务队列为空,并且运行线程执行完任务后
  /*............*/
  waitThreadPool(pool);
  //销毁任务队列
  //****需实现*****
  destory_taskqueue(&pool->queue);
  //销毁线程指针数组,并释放所有为线程池分配的内存
  /*............*/
  for (int i = 0; i < pool->num_threads; ++i) { // i 为线程 id
    free(pool->threads[i]);
  }
  free(pool->threads);
  pthread_mutex_destroy(&pool->thcount_lock);
  pthread_cond_destroy(&pool->threads_all_idle);
  free(pool);
}
/*获得当前线程池中正在运行线程的数量*/
int getNumofThreadWorking(threadpool *pool) { return pool->num_working; }
