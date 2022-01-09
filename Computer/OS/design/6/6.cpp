#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/prctl.h>
#define TRUE 1
#define FALSE 0
#define VERSION 23
#define ERROR 42
#define LOG 44
#define FORBIDDEN 403
#define NOTFOUND 404
#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif
#define HASHCOUNT 20 
#define SIZE 40
int num=0;
#define BUFSIZE 8096
pthread_mutex_t mutex;
pthread_mutex_t mutex_frame;
typedef struct content
{
    long length;//内容长度 
    char *address;//内容起始地址 
    int number;
	//此变量用于记录文件进入哈希桶的时间，将其设为全局变量。
	//每当有新文件进入桶内时，加一记录，并将其赋值给文件，便于判断文件的进入时间。 
} content;

typedef struct hashpair
{
    char *key;//文件名 
    content *cont;//内容项 
    struct hashpair *next;//在hash桶中，指向下一个hashpair 
    struct hashpair *prev;//在hash桶中，指向上一个hashpair 
} hashpair;

typedef struct hashtable
{
    hashpair **bucket;
    int num_bucket;//哈希桶的个数 
    int *capacity;//每个哈希桶的个数 
	volatile int* locks;
} hashtable;

static inline long int hashString(char *str)//字符串转化为哈希号 
{
    int hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    while(hash < 0 || hash >= HASHCOUNT)
        hash = (hash + HASHCOUNT) % HASHCOUNT;//防止哈希值过大，进行取余 
    return hash;
}

// helper for copying string keys and values
static inline char *copystring(char *value)
{
    char *copy = (char *)malloc(strlen(value) + 1);
    if (!copy)
    {
        printf("Unable to allocate string value %s\n", value);
        abort();
    }
    strcpy(copy, value);
    return copy;
}

//判断两个content是否相同，相同返回1，不同返回0 
static inline int isEqualContent(content *cont1, content *cont2)
{
    if (cont1->length != cont2->length)
        return 0;
    if (cont1->address != cont2->address)
        return 0;
    return 1;
}

int frame_fault;//全局变量，缺页数量 
int sum;//全局变量，总访问数量 
hashtable *table;
// Create hash table
hashtable *create_hash(int buckets)
{
    // allocate space
    hashtable *table = (hashtable *)malloc(sizeof(hashtable));
    if (!table)
    {
        // unable to allocate
        return NULL;
    }
    // setup
    table->bucket = (hashpair **)malloc(buckets * sizeof(void *));
    if (!table->bucket)
    {
        free(table);
        return NULL;
    }
    memset(table->bucket, 0, buckets * sizeof(void *));//清0 
    table->num_bucket = buckets;
    //初始化锁信号
    table->locks = (int *)malloc(buckets * sizeof(int));
    if (!table->locks)
    {
        free(table);
        return NULL;
    }
    memset((int *)&table->locks[0], 0, buckets * sizeof(int));
    table->capacity = (int *)malloc(buckets * sizeof(int));//哈希表的大小 
    int i;
    for ( i = 0; i < buckets; i++)
        table->capacity[i] = SIZE;//每个哈希桶大小，初始值设为40 
    return table;
}

//释放hash table中的资源 
void freeHashTable(hashtable *table)
{
    if (table == NULL)
    {
        return;
    }
    hashpair *next;
    int i;
    for (i = 0; i < table->num_bucket; i++)
    	//逐个桶释放 
    {
        hashpair *pair = table->bucket[i];//每个桶内进行逐个释放 
        while (pair)
        {
        	next=pair->next; 
            free(pair->key);
            free(pair->cont->address);
            free(pair->cont);
            free(pair);
            pair = next;
        }
    }
    free(table->bucket);
    free(table);
}

//add hashtable item
int addItem(hashtable *table, char *key, content *cont)
{
    
    // compute hash on key
    int hash = hashString(key) % table->num_bucket;//获得key在hash表中的位置 
    // get entry
    hashpair *pair = table->bucket[hash];//找到对应hash桶中的首个pair 
    pthread_mutex_lock(&mutex);
    hashpair *current=NULL; 
    while (pair)
    {
        // check for key
        if (0 == strcmp(pair->key, key) && isEqualContent(pair->cont, cont))
        {
            //exist
            pthread_mutex_unlock(&mutex);
            return 1;
        }

        if (0 == strcmp(pair->key, key) && !isEqualContent(pair->cont, cont))
        {
            //key is same,but content is different
            //重名替换 
            free(pair->cont->address);
            free(pair->cont);
            pair->cont = cont;
            pthread_mutex_unlock(&mutex);
            return 0;
        }
        
        current = pair;
        pair = pair->next;
        if (pair!= NULL)
            pair->prev = current;
    }
    //未找到对应key值，进行插入操作 
    pair = (hashpair *)malloc(sizeof(hashpair));
    pair->key = copystring(key);
    pair->cont = cont;
    pair->next = table->bucket[hash];//插入首部  最新 
    pair->prev = NULL;
    if (table->bucket[hash] != NULL)
        table->bucket[hash]->prev = pair;
    table->bucket[hash] = pair;
    table->capacity[hash] --;
    //__sync_synchronize();
    pthread_mutex_unlock(&mutex);
    return 2;
}

int deleteTail(hashtable *table, char *key) {
    int hash = hashString(key) % table->num_bucket;
    
    // get entry
    hashpair *pair = table->bucket[hash];
    hashpair *prev = NULL;
    if (pair == 0)
        return 0;
        
    while (__sync_lock_test_and_set(&table->locks[hash], 1))
	{ }
    // already an entry
    
    while (pair) //检测桶中存在的文件 找到链表最后一个存在的 
	{
        prev = pair;
        pair = pair->next;
    }
    if(prev)//存在链表最后一个文件 
	{
		table->capacity[hash] ++;
		free(prev->key);
		free(prev->cont->address);
		free(prev->cont);
		free(prev);
	} 
	__sync_synchronize();
	table->locks[hash]=0;
	return 0;
}

//根据key值，从hash表中查找相应项 
content *getContentByKey(hashtable *table, char *key)
{
    int hash = hashString(key) % table->num_bucket;
    hashpair *pair = table->bucket[hash];
    while (pair)
    {
        if (0 == strcmp(pair->key, key))
        {
            if (pair->prev != NULL && pair->next != NULL)//位于链表中间位置 ，移动到对应桶的链表头 
            {
                pair->prev->next = pair->next;
                pair->next->prev = pair->prev;
                table->bucket[hash]->prev = pair;
                pair->next = table->bucket[hash];
                pair->prev = NULL;
                table->bucket[hash] = pair;
            }
            else if(pair->prev != NULL && !pair->next)//位于链表尾部位置 ，移动到对应桶的链表头 
            {
                pair->prev->next = pair->next;
                table->bucket[hash]->prev = pair;
                pair->next = table->bucket[hash];
                pair->prev = NULL;
                table->bucket[hash] = pair;
            }

            return pair->cont;//查找成功 
        }

        pair = pair->next;
    }
    return NULL;//查找失败 
}

/*queue status and conditional variable*/ 
typedef struct staconv {
    pthread_mutex_t mutex;
    pthread_cond_t cond;/*用于阻塞和唤醒线程池中线程*/ 
    bool status;/*表示任务队列状态*/ 
} staconv;

/*Task*/
typedef struct task {
    struct task *next;/*指向下一任务*/ 
    void (*function)(void *arg);/*函数指针*/ 
    void *arg;/*函数参数指针*/ 
} task; 

/*Task Queue*/
typedef struct taskqueue {
    pthread_mutex_t mutex;/*用于互斥读/写任务队列*/ 
    task *front;/*指向队首*/ 
    task *rear;/* 指向队尾*/ 
    staconv *has_jobs;/*根据状态，阻塞线程*/ 
    int len;/*队列中任务个数*/ 
} taskqueue;  

/*Thread*/
typedef struct thread {
    int id;/*线程id*/ 
    pthread_t pthread;/*封装的POSIX线程*/ 
    struct threadpool *pool;/*与线程池绑定*/ 
} thread;

  
typedef struct threadpool {
    thread **threads;/*线程指针数组*/ 
    volatile int num_threads;/*线程池中线程数量*/ 
    volatile int num_working;/*目前正在工作的线程个数*/ 
    pthread_mutex_t thcount_lock;/*线程池锁用于修改上面两个变量*/ 
    pthread_cond_t threads_all_idle;/*用于销毁线程的条件变量*/ 
    taskqueue queue;/*任务队列*/ 
    volatile bool is_alive;/*表示线程池是否还存活*/ 
} threadpool;
  
int create_thread(struct threadpool* pool,struct thread* pthread,int id);
void* thread_do(struct thread* pthread);

/*初始化任务队列*/
void init_taskqueue(taskqueue* queue)
{
	 queue->len = 0;
	 queue->front =NULL;
	 queue->rear=NULL;
     queue->has_jobs = (staconv*)malloc(sizeof(struct staconv));
	 queue->has_jobs->status = FALSE;
	 pthread_mutex_init(&(queue->mutex), NULL);
     pthread_cond_init(&(queue->has_jobs->cond), NULL);
     pthread_mutex_init(&(queue->has_jobs->mutex), NULL);
}


/*线程池初始化函数*/ 
struct threadpool* initThreadPool(int num_threads) {
	//创建线程空间 
   	threadpool* pool;
	pool=(threadpool*)malloc(sizeof(struct threadpool));
	pool->num_threads=0;
	pool->num_working=0;
	//初始化互斥量和条件变量 
	pthread_mutex_init(&(pool->thcount_lock),NULL);
	pthread_cond_init(&(pool->threads_all_idle),NULL);
	pool->is_alive=TRUE;
    //初始化任务队列 
	init_taskqueue(&(pool->queue));
    //创建线程数组 
	pool->threads=(struct thread**)malloc(num_threads * sizeof(struct thread*));
	//创建线程 
	int i;
	for(i=0;i<num_threads;i++)
	{
		create_thread(pool,pool->threads[i],i);//i为线程id 
	}
	while(pool->num_threads!=num_threads){}
	return pool;
}

 /*将任务加入队列*/
void push_taskqueue(taskqueue* queue,task* curtask) //任务队列 当前任务 
{
	pthread_mutex_lock(&(queue->mutex));//用于互斥读/写任务队列
	if(queue->len==0)//任务队列中任务数为0
	{
		queue->front=curtask;
		queue->front->next = queue->rear;
		queue->rear=curtask;
	} 
	else
	{
		queue->rear->next=curtask;
		queue->rear=queue->rear->next;
	} 
	queue->len++;
	pthread_mutex_lock(&(queue->has_jobs->mutex));
	queue->has_jobs->status =TRUE;
	pthread_cond_signal(&(queue->has_jobs->cond));//用于唤醒线程池中的线程
	pthread_mutex_unlock(&(queue->has_jobs->mutex));
	pthread_mutex_unlock(&(queue->mutex));
}

/*向线程池中添加任务*/ 
void addTask2ThreadPool(threadpool* pool, task* curtask) {
    push_taskqueue(&pool->queue, curtask);
}

/*等待当前任务全部运行完*/ 
void waitThreadPool(threadpool* pool) {
    pthread_mutex_lock(&pool->thcount_lock);
    while (pool->queue.len || pool->num_working) {
        pthread_cond_wait(&pool->threads_all_idle, &pool->thcount_lock);
    }
    pthread_mutex_unlock(&pool->thcount_lock);
}

/*销毁任务队列*/
void destory_taskqueue(taskqueue* queue)
{
	
	pthread_mutex_lock(&(queue->mutex));
	task* curtask;
	while(queue->front != NULL)
	{
		curtask = queue->front;
		queue->front = queue->front->next;
		free(curtask);
	}	
	
	pthread_mutex_lock(&(queue->has_jobs->mutex));
	queue->has_jobs->status = FALSE;
	pthread_mutex_unlock(&(queue->has_jobs->mutex));
	pthread_mutex_unlock(&(queue->mutex));
	
    pthread_mutex_destroy(&(queue->has_jobs->mutex));
    pthread_cond_destroy(&(queue->has_jobs->cond));
	pthread_mutex_destroy(&(queue->mutex));
}

/*销毁线程池*/ 
void destoryThreadPool(threadpool* pool){
//如果当前任务队列中有任务，需等待任务队列为空，并且运行线程执行完任务后
    
	pthread_mutex_lock(&pool->thcount_lock);
    waitThreadPool(pool);//等待当前任务全部运行完
    destory_taskqueue(&pool->queue);//销毁任务队列
//销毁线程指针数组，并释放所有为线程池分配的内存
    int i;
    for (i = 0; i < pool->num_threads; ++i)
        free(pool->threads[i]);
    free(pool->threads);
    pool->is_alive = FALSE;
	pthread_mutex_unlock(&pool->thcount_lock);
    pthread_mutex_destroy(&(pool-> thcount_lock));//销毁锁
    pthread_cond_destroy(&(pool->threads_all_idle));//销毁条件变量
}

/*获得当前线程池中正在运行线程的数量*/ 
int getNumofThreadWorking(threadpool* pool) {
    return pool->num_working;
}


/*从任务队列头部提取任务，并在队列中删除此任务*/ 
task* take_taskqueue(taskqueue* queue)
{
	pthread_mutex_lock(&queue->mutex);
	if (queue->len != 0)//队列中存在任务 
	{
		task* curtask = queue->front;
		queue->front = queue->front->next;
		queue->len--;
		if (queue->len == 0)
		{
			pthread_mutex_lock(&queue->has_jobs->mutex);
			queue->has_jobs->status = FALSE;
			pthread_mutex_unlock(&queue->has_jobs->mutex);
		}
		else
		{
			pthread_cond_signal(&queue->has_jobs->cond);//唤醒线程 
		}
		pthread_mutex_unlock(&queue->mutex);
		return curtask;
	}
	else
	{
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
}
/*创建线程*/ 
int create_thread(struct threadpool* pool,struct thread* pthread,int id)
{
	pthread=(struct thread*)malloc(sizeof(struct thread));
	if(pthread==NULL)
	{
		perror("creat_thread(): Could not allocate memory for thread\n");
		return -1;
	}
	pthread->pool=pool;
	pthread->id=id;
	pthread_create(&(pthread->pthread),NULL,(void *)thread_do,pthread);
	pthread_detach(pthread->pthread);
	return 0;
}

/*线程运行的逻辑函数*/ 
void* thread_do(struct thread* pthread){
	/*设置线程名字*/
	char thread_name[128]={0};
	sprintf(thread_name,"thread-poop-%d",pthread->id);
	
	prctl(PR_SET_NAME,thread_name);
	
	/*获得线程池*/
	threadpool* pool=pthread->pool;
	
	/*在线程池初始化时，用于已经创建线程的计数，执行pool->num_thread++*/
	
	pthread_mutex_lock(&(pool->thcount_lock));//线程池锁，用于修改线程池中线程数量变量 
	pool->num_threads++;
	pthread_mutex_unlock(&(pool->thcount_lock));
	/*线程一直循环往复运行，直到pool->is_alive变为false*/
	while(pool->is_alive){

		/*如果任务队列中还有任务，则继续运行，否则阻塞*/
		pthread_mutex_lock (&(pool->queue.has_jobs->mutex));//用于互斥读写任务队列 
		 while ((pool->queue.has_jobs->status == 0) &&pool->is_alive)//无任务状态 
         {
             pthread_cond_wait (&(pool->queue.has_jobs->cond), &(pool->queue.has_jobs->mutex));//阻塞 
         }
		pthread_mutex_unlock (&(pool->queue.has_jobs->mutex));
		
		if(pool->is_alive){ 
		/*执行到此位置，表明线程在工作，需要对工作线程数量进行计数*/
		//pool->num_working++
		pthread_mutex_lock (&(pool->thcount_lock));//线程池锁，用于修改线程池中正在工作线程数量变量 
        pool->num_working++;
        pthread_mutex_unlock (&(pool->thcount_lock));

		/*从任务队列的队首提取任务并执行*/
		  void(*func)(void*);
		  void* arg;
		  //take_taskqueue从任务队列头部提取任务，并在队列中删除此任务
		   task* curtask=take_taskqueue(&pool->queue);
			if(curtask)
			{
				func=curtask->function;
				arg=curtask->arg;
				//执行任务 
				func(arg);
				//释放任务 
				free(curtask);
			}
			/* 执行到此位置，表明线程已经将任务执行完成，需要改工作线程数量*/
			/*此处还需注意，当工作线程数量为0，表示任务全部完成，要让阻塞在waitThreadPool()函数上的线程继续运行*/
			pthread_mutex_lock (&(pool->thcount_lock));//线程池锁，用于修改线程池中正在工作线程数量变量 
    		pool->num_working--;
			pthread_mutex_unlock(&(pool->thcount_lock));
			if(pool->num_working == 0)//任务全部完成 
				pthread_cond_signal(&(pool->threads_all_idle));//唤醒线程 
		} 
	}
	/*运行到此位置表明，线程将要退出，需更改当前线程池中的线程数量*/ 
	//pool->num_threads--;
	pthread_mutex_lock(&(pool->thcount_lock));//线程池锁，用于修改线程池中线程数量变量 
	pool->num_threads--;
	pthread_mutex_unlock(&(pool->thcount_lock));
	return NULL; 
}


struct
{
    char *ext;
    char *filetype;
} extensions[] = {
    {"gif", "image/gif"},
    {"jpg", "image/jpg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"ico", "image/ico"},
    {"zip", "image/zip"},
    {"gz", "image/gz"},
    {"tar", "image/tar"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {0, 0}};

typedef struct
{
    int hit;
    int fd;
} webparam;

unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }
    return filesize;
}

 
void logger(int type, char* s1, char* s2, int socket_fd) 
{
    int fd;
    char logbuffer[BUFSIZE * 2];
    
    switch (type) {
        case ERROR:
            (void) sprintf(logbuffer, "ERROR: %s:%s Errno=%d exiting pid=%d", s1, s2, errno, getpid());
            break;
        case FORBIDDEN:
            (void) write(socket_fd,
                         "HTTP/1.1 403 Forbidden\nContent-Length: 185\nConnection:close\nContent-Type:text/html\n\n<html><head>\n<title>403 Forbidden</title>\n</head><body>\n<h1>Forbidden</h1>\nThe requested URL, file type or operation is not allowed on this simple static file webserver.\n</body></html>\n",
                         271);
            (void) sprintf(logbuffer, "FORBIDDEN: %s:%s", s1, s2);
            break;
        case NOTFOUND:
            (void) write(socket_fd,
                         "HTTP/1.1 404 Not Found\nContent-Length: 136\nConnection:close\nContent-Type:text/html\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n",
                         224);
            (void) sprintf(logbuffer, "NOT FOUND: %s:%s", s1, s2);
            break;
        case LOG:
            (void) sprintf(logbuffer, " INFO: %s:%s:%d", s1, s2, socket_fd);
            break;
    }
/* No checks here, nothing can be done with a failure anyway */
    if ((fd = open("webserver.log", O_CREAT | O_WRONLY | O_APPEND, 0644)) >= 0) {
        (void) write(fd, logbuffer, strlen(logbuffer));
        (void) write(fd, "\n", 1);
        (void) close(fd);
    }
//if(type == ERROR || type == NOTFOUND || type == FORBIDDEN) exit(3);
}

/* this is a web thread, so we can exit on errors */
/*此函数完成了webserver主要功能，它首先解析客户端发送的消息，然后从中获取客户端请求的文件名，
然后根据文件名从本地将此文件读入缓存，并生成相应的HTTP响应消息；最后通过服务器与客户端的socket
通道想客户端返回HTTP响应消息*/ 
void web(void *data) {

    int fd;
    int hit;
    int j, file_fd, buflen;
    long i, ret, len;
    char *fstr;
    char buffer[BUFSIZE + 1]; /* static so zero filled *//*设置静态缓冲区*/ 
    webparam *param = (webparam *) data;
    fd = param->fd;
    hit = param->hit;
    
    ret = read(fd, buffer, BUFSIZE); /* read web request in one go *//*从连接通道中读取客户端的请求消息*/ 
    if (ret == 0 || ret == -1) 
	{ /* read failure stop now */
	  /*如果读取客户端消息失败，则向客户端发送HTTP失败响应信息*/ 
        logger(FORBIDDEN, "failed to read browser request", "", fd);
    } 
	else 
	{
        if (ret > 0 && ret < BUFSIZE) /* return code is valid chars *//*设置有效字符串，即将字符串尾部表示为0*/ 
            buffer[ret] = 0;/* terminate the buffer */
        else buffer[0] = 0;
        for (i = 0; i < ret; i++) /* remove cf and lf characters */
            if (buffer[i] == '\r' || buffer[i] == '\n')
                buffer[i] = '*';
        logger(LOG, "request", buffer, hit);
        /*判断客户端HTTP请求消息是否为GET类型，如果不是则给出响应的响应消息*/ 
        if (strncmp(buffer, "GET ", 4) && strncmp(buffer, "get ", 4)) 
		{
            logger(FORBIDDEN, "only simple get operation supported", buffer, fd);
        }
        for (i = 4; i < BUFSIZE; i++) 
		{ /* null terminate after the second space to ignore extra stuff */
            if (buffer[i] == ' ') 
			{ /* string is "get url " +lots of other stuff */
                buffer[i] = 0;
                break;
            }
        }
        for (j = 0; j < i - 1; j++) /* check for illegal parent directory use .. */
            if (buffer[j] == '.' && buffer[j + 1] == '.') 
			{
                logger(FORBIDDEN, "parent directory (..) path names not supported", buffer, fd);
            }
        if (!strncmp(&buffer[0], "GET /\0", 6) || !strncmp(&buffer[0], "GET /\0", 6)) /* convert no filename to index file */
        /*如果请求消息中没有包含有效的文件名，则使用默认文件名index.html*/ 
            (void) strcpy(buffer, "GET /index.html");
        /* work out the file type and check we support it */
        /*检查请求服务器类型是否支持*/ 
        buflen = strlen(buffer);
        fstr = (char *) 0;
        for (i = 0; extensions[i].ext != 0; i++) 
		{
            len = strlen(extensions[i].ext);
            if (!strncmp(&buffer[buflen - len], extensions[i].ext, len)) 
			{
                fstr = extensions[i].filetype;
                break;
            }
        }
        if (fstr == 0) 
		logger(FORBIDDEN, "file extension type not supported", buffer, fd);
		
		char *temp=malloc(sizeof(char)*1024);
		memcpy(temp,buffer,sizeof(char)*1024); 
        
        content *cont = getContentByKey(table, &buffer[5]);  //判断是否在哈希表中
        if (cont == NULL)//不在情况 
		{
			content *contprecent = (content *)malloc(sizeof(content));
			if (contprecent == NULL)
                free(temp);
            else
            {
            	if ((file_fd = open(&buffer[5], O_RDONLY)) == -1) 
				{ /* open the file for reading */
				/*打开指定文件名*/ 
                   logger(NOTFOUND, "failed to open file", &buffer[5], fd);
                }
                logger(LOG, "send", &buffer[5], hit);
                len = (long) lseek(file_fd, (off_t) 0, SEEK_END); /* 使用 lseek 来获得文件长度,比较低效*/
                (void) lseek(file_fd, (off_t) 0, SEEK_SET);
                (void) sprintf(buffer,
                       "http/1.1 200 ok\nserver: nweb/%d.0\ncontent-length: %ld\nconnection:close\ncontent-type: %s\n\n",
                       VERSION, len, fstr); /* header + a blank line */
		        logger(LOG, "header", buffer, hit);
                (void) write(fd, buffer, strlen(buffer));
                
                char *filetext = malloc(sizeof(char) * (len + 1));
                if (table->capacity[hashString(&temp[5]) % HASHCOUNT] == 0)//如果哈希桶中剩余为空，则删除一项，后续添加 
                {
                	deleteTail(table, &temp[5]); 
				}
				if (len > 0) //添加 
				{
					pthread_mutex_lock(&mutex_frame);
					frame_fault++;
					sum++;
					printf("sum= %d    frame_fault= %d      fault%= %f\n",sum,frame_fault,(1.0*frame_fault)/sum);
					pthread_mutex_unlock(&mutex_frame);
                    read(file_fd, filetext, (len + 1));
                    contprecent->length = len;
                    contprecent->address = filetext;
                    (void)write(fd, filetext, len);
                    addItem(table, &temp[5], contprecent);  //将新来的任务添加到哈希表中
                }
                 close(file_fd);

			}


		}
		else //如果已经在哈希表中，直接读取网页内容
		{
			pthread_mutex_lock(&mutex_frame);
			sum++;
			printf("sum= %d    frame_fault= %d      fault%= %f\n",sum,frame_fault,(1.0*frame_fault)/sum);
			num++;
			cont->number=num; //访问时间 
			pthread_mutex_unlock(&mutex_frame);
			logger(LOG, "send", &buffer[5], hit);                                                                                                                   
            (void)sprintf(buffer, "HTTP/1.1 200 OK\nServer: nweb/%d.0\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", VERSION, cont->length, fstr); 
             logger(LOG, "header", buffer, hit);
            (void)write(fd, buffer, strlen(buffer));
			(void)write(fd, cont->address, cont->length);
		}

        sleep(0.00001);/*在 socket 通道关闭前,留出一段信息发送的时间*/
        close(file_fd);
    }
    close(fd);
//释放内存
    free(param);
}

int main(int argc, char **argv)
{
	signal(SIGPIPE,SIG_IGN);
	frame_fault=0;
	sum=0;
	table=create_hash(HASHCOUNT);

    struct threadpool* pool;
    int i, port, pid, listenfd, socketfd, hit;
    socklen_t length;
    static struct sockaddr_in cli_addr; /* static = initialised to zeros */
    static struct sockaddr_in serv_addr; /* static = initialised to zeros */
    
    if (argc < 3 || argc > 3 || !strcmp(argv[1], "-?")) {
        (void) printf("hint: nweb Port-Number Top-Directory\t\tversion %d\n\n"
                      "\tnweb is a small and very safe mini web server\n"
                      "\tnweb only servers out file/web pages with extensions named below\n"
                      "\t and only from the named directory or its sub-directories.\n"
                      "\tThere is no fancy features = safe and secure.\n\n"
                      "\tExample: nweb 8181 /home/nwebdir &\n\n"
                      "\tOnly Supports:", VERSION);
        for (i = 0; extensions[i].ext != 0; i++)
            (void) printf(" %s", extensions[i].ext);
            
        (void) printf("\n\tNot Supported: URLs including \"..\", Java, Javascript, CGI\n"
                      "\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev /sbin \n"
                      "\tNo warranty given or implied\n\tNigel Griffiths nag@uk.ibm.com\n");
        exit(0);
    }
    if (!strncmp(argv[2], "/", 2) || !strncmp(argv[2], "/etc", 5) ||
        !strncmp(argv[2], "/bin", 5) || !strncmp(argv[2], "/lib", 5) ||
        !strncmp(argv[2], "/tmp", 5) || !strncmp(argv[2], "/usr", 5) ||
        !strncmp(argv[2], "/dev", 5) || !strncmp(argv[2], "/sbin", 6)) {
        (void) printf("ERROR: Bad top directory %s, see nweb -?\n", argv[2]);
        exit(3);
    }
    if (chdir(argv[2]) == -1) {
        (void) printf("ERROR: Can't Change to directory %s\n", argv[2]);
        exit(4);
    }
    
    pool=initThreadPool(40);//设置线程池中线程个数为40
    logger(LOG, "nweb starting", argv[1], getpid());
/* setup the network socket */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        logger(ERROR, "system call", "socket", 0);
    port = atoi(argv[1]);
    if (port < 0 || port > 60000)logger(ERROR, "Invalid port number (try 1->60000)", argv[1], 0);
//初始化线程属性,为分离状态
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t pth;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        logger(ERROR, "system call", "bind", 0);
    if (listen(listenfd, 64) < 0)
        logger(ERROR, "system call", "listen", 0);
    for (hit = 1;; hit++) {
        length = sizeof(cli_addr);
        if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0)
            logger(ERROR, "system call", "accept", 0);
       
          	webparam *param=malloc(sizeof(webparam));
          	param->hit=hit;  
          	param->fd=socketfd;
		    struct task* curtask=(task*)malloc(sizeof(struct task));
	    	curtask->next=NULL;
		    curtask->function=web;//函数指针 
		    curtask->arg=(void*)param;//函数参数指针 
	     	addTask2ThreadPool(pool,curtask);  
    }
    destoryThreadPool(pool);
    freeHashTable(table);
}
