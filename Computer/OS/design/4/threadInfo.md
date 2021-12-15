#

- include <pthread.h>

/*Linux 提供了兼容 POSIX 标准的线程操作 API,其主要的函数如下所示
- int pthread_create(thread_t* thread, pthread_attr_t* attr, void*
(start_routine)(void*), void* arg) 线程创建函数,其中参数 thread
是创建好线程的指针,用于后续的线程操作;
atrr 为线程属性指针,如果为 NULL,将按默认属性来创建线程;
start_routine 为完成线程逻辑功能的函数指针;
arg 为向线程传递参数的指针。
如果成功返回 0,失败返回-1。

- void pthread_exit(void* retval)
  退出当前线程函数,参数 retval 用来返回当前函数的退出值。
- int pthread_cancel(pthread_t thread)
  向目标线程发送请求终止(cancel)信号,其中参数 thread 为要被取消运行的线程 id。
  当然调用 pthread_cancel 并不意味着目标的线程一定要被终止,而是目标线程接收到
  cancel
信息后,它自己决定如何来响应这个信号:忽略这个信号、立即退出、运行至取消点
(cancelation-point)后再退出。 在此函数发出 cancel 信号后, 目标线程的 cancelstate
来决定是否接受此 cancel 信号, 如果 cancel state 是 PTHREAD_CANCEL_ENABLE (默认)
则 接 收 信 号 ; 如果是 PTHREAD_CANCEL_DISABLE , 则 不 接 收 此 信 号 。对
cancel state 的 设 置 使 用

  “ int pthread_setcancelstate(int state, int *oldstate) ”函数,其中参数 state
可以设置为上述两种状态之一。 当目标线程接收到 cancel state 信号,目标线程的
cancel type 来决定何时取消。 如果 cancel type 是 PTHREAD_CANCEL_DEFERRED(默认)
,目标线程并不会马上取消,而是在执行 下 一 条 cancellation point 的 时 候 才 会 取
消 ; 如 果 cancel type 是 PTHREAD_CANCEL_ASYNCHRONOUS, 目标线程会立即取消。

- 对 cancel type 的设置使用“int pthread_setcanceltype(int type, int *oldtype)
”函数,其中参数 type 为上述两种数值之一。 而 cancellation point 为 调 用 POSIX 库
中 pthread_join 、 pthread_testcancel 、
pthread_cond_wait、pthread_cond_timedwait、sem_wait、sigwait 以及 read、write
等函数的位置。 int pthread_join(pthread_t* tid, void** thread_return)
等待线程结束函数,其中参数 tid 为被等待的线程 id 指针;
thread_return 为被等待线程的返回值,也就是 pthread_exit 里面的参数值。
如果当前线程调用此函数,将会阻塞,直到被等待线程运行结束后或者被其它线程取消运行,当前线程才会继续运行。
另外需要注意的是,一个线程不能被多个线程等待,否则除第一个等待线程外,其它等待线程均会返回错误值
ESRCH。


# 

对与 pthread 的属性设置,提供了如下的接口函数。

- int pthread_attr_init (pthread_attr_t* attr)
属性初始化函数,创建一个线程属性结构,并通过 attr 指向此结构。
- int pthread_attr_destroy(pthread_attr_t *attr); 销毁一个线程属性结构。
- int pthread_attr_setscope (pthread_attr_t* attr, int scope);
  设置线程的作用域,在 POSIX 标准中参数 scope 可以取 PTHREAD_SCOPE_SYSTEM 和
PTHREAD_SCOPE_PROCESS。 这两个值分别表示线程调度范围是在系统中还是在进程中。
  但是在 Linux 中仅支持 PTHREAD_SCOPE_SYSTEM(也就是“一对一”模型)
  如果 scope 设置 PTHREAD_SCOPE_PROCESS,则会报错,返回 ENOTSUP。

- int pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate)
  设置分离属性。 创建的线程分为分离和非分离状态。
  如果线程为分离状态(detachstate 取值为 PTHREAD_CREATE_DETACHED)
则线程在运行完就自行结束并释放资源; 如果线程为非分离状态 (detachstate 取值为
PTHREAD_CREATE_JOINABLE) 则此线程需要等待它的线程中 pthread_joint
  函数返回后,才终止并释放资源。线程默认为非分离状态。

 如果将线程状态设置为分离状态,需要注意的是创建的线程运行可能非常快,在
pthread_create 函数没有返回时已经运行结束,这时 pthread_create
中可能会得到操作的线程号。 为了避免这个问题,可以在创建线程里调用
pthread_cond_timewait 函数,让线程等待一会。
*/

    /*• int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize,
     * const cpu_set_t *cpuset)*/

pthread_attr_t attr1;
pthread_attr_init(&attr1);
cpu_set_t cpu_info;
__CPU_ZERO(&cpu_info);
__CPU_SET(0, &cpu_info);
pthread_attr_setaffinity_np(&attr1, sizeof(cpu_set_t), &cpu_info);

/*• int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)*/
/*设置线程的调度策略。policy 可以取值 SCHED_FIFO、SCHED_RR 和 SCHED_OTHER。其*/
/*中 SCHED_OTHER 为默认的分时调度策略,
 * 表示线程一旦开始运行,直到时间片运行完或者*/
/*阻塞或者运行结束才让出 CPU 控制权,此状态下不支持线程的优先级。 SCHED_FIFO
 * 为实时*/
/*调度,执行先来先服务的调度策略,一个线程一旦占有
 * CPU,则运行到阻塞或者有更高优先*/
/*级的线程到来。SCHED_RR 为实时调度,执行时间片轮转调度。*/
/*- int pthread_attr_setschedparam(pthread_attr_t *attr, const struct
 * sched_param *param)*/
/*设置线程的优先级。其中 sched_param 结构中仅有属性
 * sched_priority,其用来设置线*/
/*程的优先级。线程优先级可以取 1-99 中任意个数字,数值越大优先级越高。*/
pthread_attr_t attr;
struct sched_param param;
pthread_attr_init(&attr);
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
param.sched_priority = 50;
pthread_attr_setschedparam(&attr, &param);

# 线程间同步和互斥还可以使用无名信号量来实现。有关无名信号量的创建、操作和销毁函数见进程模型。

互斥量操作 互斥量可以用来实现临近区,让线程互斥地使用临界资源。在 linux 的 POSIX
库中,提 供 pthread_mutex_init、 pthread_mutex_lock、 pthread_mutex_trylock、
pthread_mutex_unlock 和 pthread_mutex_destroy
 等函数来完成互斥量的初始化、加锁、释放、摧毁等操作。
其中 pthread_mutex_trylock
为非阻塞函数,如果互斥量没有被锁住,其对互斥量加锁,并进入临界区;
如果互斥量已经加锁, 则返回 EBUSY,而不会阻塞。 ???????干啥呢
pthread_mutex_lock 为阻塞函数,
如果已经有其它线程占有互斥量,则阻塞直到获得这个互斥量为止。

# 读写锁
对于读写者问题 (多个读线程能同时读取数据, 只有写线程写入数据时才会阻塞其它线程)
, POSIX 库提供了 pthread_rwlock_init 和 pthread_rwlock_destroy
函数用来创建和销毁读写锁; pthread_rwlock_rdlock、 pthread_rwlock_wrlock 和
pthread_rwlock_timedrdlock 函数使 用阻塞的方式来获得读锁或者写锁;
pthread_rwlock_tryrdlock 和 pthread_rwlock_trywrlock
函数使用非阻塞方式来获得读锁或写锁;
pthread_rwlock_unlock 函数释放读写锁。具体用


# 

- 二者的关系操作可以形象看作从一个无序的占用资源变成一个有序的节约资源的一个改变，所以条件变量和互斥锁二者使用能够保证条件变量的正确修改。

条件变量用于某个进程或线程等待某个信号条件到来时才继续运行的场景。
POSIX 库中 提供了 pthread_cond_init 和 pthread_cond_destroy
函数用来完成条件变量的创建和销毁; 提供 pthread_cond_wait 和
pthread_cond_timewait 函数用来完成线程等待或限时等待在某个条件量上,
函数中的参数为条件变量和互斥量,
以上函数将利用互斥量来完成对条件变量状态的修改,
以保证多线程状态下条件变量一致性,
因此在调用这两个函数之前,一定要获得这个 互斥量的资源,
即在此函数调用前一定要有互斥量的加锁操作;
提供 pthread_cond_signal 和 pthread_cond_broadcast
函数用来唤醒等待在条件变量上的一个线程或所有线程。

- pthread_cond_wait() 函数一进入wait状态就会自动release mutex
- pthread_cond_wait() 一旦wait成功获得cond 条件的时候会自动 lock mutex.
- 为什么要与pthread_mutex 一起使用呢？ 这是为了应对线程1在调用pthread_cond_wait()但线程1还没有进入wait cond的状态的时候，此时线程2调用了 cond_singal 的情况。 如果不用mutex锁的话，这个cond_singal就丢失了。加了锁的情况是，线程2必须等到 mutex 被释放（也就是 pthread_cod_wait() 进入wait_cond状态 并自动释放mutex） 的时候才能调用cond_singal.

> 可以调用pthread_cancel()来退出线程1。那样显然线程1会在 pthread_cond_wait(&m_cond,&m_mutex);    和 pthread_mutex_unlock(&m_mutex); 之间退出，    pthread_cond_wait() 函数返回后自动lock住了mutex, 这个时候线程1退出（并没有运行到pthread_mutex_unlock（）），如果Thread2这个时候就再也得不到lock状态了。

通常解决这个问题的办法如下

        void cleanup(void *arg)
        {
           pthread_mutex_unlock(&mutex);
        }
        void * thread1(void * arg)
        {
            pthread_cleanup_push(cleanup, NULL); // thread cleanup handler 
            pthread_mutex_lock(&mutex);
           pthread_cond_wait(&cond, &mutex);
           pthread_mutex_unlock(&mutex);
           pthread_cleanup_pop(0 );
        }

# sema

- “这段共享内存的使用是有竞争条件存在的，从文件锁的例子我们知道，进程间的通信绝不仅仅是通信那么简单，还需要在处理类似这样的临界区代码。在这里，我们也可以使用文件锁来处理，但是共享内存使用文件锁未免显得太不协调了。除了不方便以及效率低下以外，文件锁还不能够进行更高的进程控制。所以，我们在此需要更高级的进程同步控制原语来实现相关功能，这就是信号量的作用”
