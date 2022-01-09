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
#define LOG 44
#define BUFSIZE 8096
#define ERROR 42
#define LOGTIME 46
#define FORBIDDEN 403
#define NOTFOUND 404

#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif

/*queue status and conditional variable*/
typedef struct staconv
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;/*用于阻塞和唤醒线程池中线程   条件变量*/ 
	int status; /*表示任务队列状态*/ 
} staconv;
 
/*Task*/
typedef struct task
{
 	struct task* next;/*指向下一任务*/ 
	 void  (*function)(void* arg);/*函数指针*/ 
	 void* arg;/*函数参数指针*/ 
} task;
  
  /*Task Queue*/
typedef struct taskqueue
{
  	  pthread_mutex_t mutex;/*用于互斥读/写任务队列*/ 
	  task *front;/*指向队首*/ 
	  task *rear;/* 指向队尾*/ 
	  staconv *has_jobs;/*根据状态，阻塞线程*/ 
	  int len;/*队列中任务个数*/ 
}taskqueue;
  
/*Thread*/
typedef struct thread
{
  	  int id;/*线程id*/ 
	  pthread_t pthread;/*封装的POSIX线程*/ 
	  struct threadpool* pool; /*与线程池绑定*/ 
}thread;
  
/*Thread pool*/
typedef struct threadpool
{
  	thread** threads;/*线程指针数组*/ 
  	volatile int num_threads;/*线程池中线程数量*/ 
  	volatile int num_working;/*目前正在工作的线程个数*/ 
	pthread_mutex_t thcount_lock;/*线程池锁用于修改上面两个变量*/ 
	pthread_cond_t threads_all_idle; /*用于销毁线程的条件变量*/ 
	taskqueue queue;/*任务队列*/ 
	volatile bool is_alive;/*表示线程池是否还存活*/ 
	volatile int max_working;/*线程最高活跃数量*/ 
	volatile int min_working;/*线程最低活跃数量*/ 
	struct timeval start;/*起始时间*/ 
	struct timeval end;/*终止时间*/ 
	int id;/*线程池id*/ 	
}threadpool;
  
struct threadpool* readmsg_threadpool;
struct threadpool* readfile_threadpool;
struct threadpool* sendmsg_threadpool;
void *web1(void *data);
void *web2(void *data);
void *web3(void *data);

int create_thread(struct threadpool* pool,struct thread* pthread,int id);
void* thread_do(struct thread* pthread);

int a[5]={0,0,0,0,0};

void init_taskqueue(taskqueue* queue)
{
	 queue->len = 0;
   	 queue->front=(task*)malloc(sizeof(struct task));
	 queue->rear=queue->front;
	 queue->front->next=NULL;
     queue->has_jobs = (staconv*)malloc(sizeof(struct staconv));
	 queue->has_jobs->status = FALSE;
	 pthread_mutex_init(&(queue->mutex), NULL);
     pthread_cond_init(&(queue->has_jobs->cond), NULL);
     pthread_mutex_init(&(queue->has_jobs->mutex), NULL);
} 

/*线程池初始化函数*/ 
struct threadpool* initThreadPool(int num_threads,int id)
{
  	//创建线程空间 
	  threadpool* pool;
	  pool=(threadpool*)malloc(sizeof(struct threadpool));
	  pool->num_threads=0;
	  pool->num_working=0;
	  pool->max_working=0;
	  pool->min_working=1000;
	  pool->id=id;/*区分不同的线程池*/ 
	  gettimeofday(&(pool->start),NULL);
	  gettimeofday(&(pool->end),NULL);
	  //初始化互斥量和条件变量 
	  pthread_mutex_init(&(pool->thcount_lock),NULL);
	  pthread_cond_init(&(pool->threads_all_idle),NULL);
	  pool->is_alive=TRUE;
      //初始化任务队列 
	  init_taskqueue(&(pool->queue));
	  //创建线程数组 
	  pool->threads=(struct thread **)malloc(num_threads*sizeof(struct thread*));
	  //创建线程 
	  for(int i=0;i<num_threads;i++)
	  {
	  	create_thread(pool,pool->threads[i],i);//i为线程id  
	   } 
	   while(pool->num_threads!=num_threads){
	   } 
	   return pool;
} 

 /*将任务加入队列*/
 void push_taskqueue(taskqueue *queue,task *curtask)//任务队列 当前任务 
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
void addTask2ThreadPool(threadpool* pool,task* curtask)
{
   	push_taskqueue(&(pool->queue),curtask); 
}

/*等待当前任务全部运行完*/ 
void waitThreadPool(threadpool* pool)
{
   	pthread_mutex_lock(&pool->thcount_lock);
   	while(pool->queue.len||pool->num_working)
   	{
   		pthread_cond_wait(&pool->threads_all_idle,&pool->thcount_lock);
	}
	pthread_mutex_unlock(&(pool->thcount_lock));
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
 void* thread_do(struct thread* pthread)
  {
  	/*设置线程名字*/
	char thread_name[128]={0};
	float wake_time;/*线程活跃时间*/ 
	float sleep_time;/*线程阻塞时间*/
	struct timeval wake_start;
	struct timeval wake_end;
	struct timeval sleep_start;
	struct timeval sleep_end;
	sprintf(thread_name,"thread-pool-%d",pthread->id); 
	  
	prctl(PR_SET_NAME,thread_name);
	  
	/*获得线程池*/	  
	threadpool* pool=pthread->pool;

	/*在线程池初始化时，用于已经创建线程的计数，执行pool->num_thread++*/
	pthread_mutex_lock(&(pool->thcount_lock));
	pool->num_threads++;
	pthread_mutex_unlock(&(pool->thcount_lock));
	  
	/*线程一直循环往复运行，直到pool->is_alive变为false*/
	 while(pool->is_alive)
	{
	    /*如果任务队列中还有任务，则继续运行，否则阻塞*/ 
		pthread_mutex_lock(&(pool->queue.has_jobs->mutex));//用于互斥读写任务队列 
		sleep_time=0;
		while ((pool->queue.has_jobs->status == 0) &&pool->is_alive)//无任务状态 
         {
         	gettimeofday(&(sleep_start),NULL);//线程阻塞开始时间 
            pthread_cond_wait (&(pool->queue.has_jobs->cond), &(pool->queue.has_jobs->mutex));//阻塞 
            gettimeofday(&(sleep_end),NULL);//线程阻塞结束时间 
			sleep_time=1000000*((sleep_end).tv_sec-(sleep_start).tv_sec)+(sleep_end).tv_usec-(sleep_start).tv_usec;
			sleep_time=sleep_time/1000;
         }
		pthread_mutex_unlock (&(pool->queue.has_jobs->mutex));
		
		if(pool->is_alive){ 
		/*执行到此位置，表明线程在工作，需要对工作线程数量进行计数*/
		//pool->num_working++
		
		gettimeofday(&(wake_start),NULL);/*线程活跃开始时间*/ 
		pthread_mutex_lock (&(pool->thcount_lock));//线程池锁，用于修改线程池中正在工作线程数量变量 
        pool->num_working++;
        
		if((pool->num_working)>(pool->max_working))
		{/*记录线程最高活跃数量*/ 
			pool->max_working=pool->num_working;
        }  
		if((pool->num_working)<(pool->min_working))
		{/*记录线程最低活跃数量*/ 
			pool->min_working=pool->num_working;				
        }
	    pthread_mutex_unlock(&(pool->thcount_lock));
	
        /*从任务队列的队首提取任务并执行*/
		void(*func)(void*);
		void* arg;
		 //take_taskqueue从任务队列头部提取任务，并在队列中删除此任务
		task* curtask=take_taskqueue(&(pool->queue));
		if(curtask)
		{
			func=curtask->function;
			arg=curtask->arg;
			//执行任务 
			func(arg);
			//释放任务 
			free(curtask);	
		} 
		
		gettimeofday(&(wake_end),NULL);/*线程活跃结束时间*/ 
		wake_time=1000000*((wake_end).tv_sec-(wake_start).tv_sec)+(wake_end).tv_usec-(wake_start).tv_usec;
		wake_time=wake_time/1000;

		if(pool->id==1){
		printf("****************************************************\n");
		printf("pool_id:%d  \n1.readmsgpool:wake_time=%f  sleep_time=%f\n",pool->id,wake_time,sleep_time);
		printf("2.readmsgpool:max_working=%d min_working=%d \n",pool->max_working,pool->min_working);	
		printf("3.readmsgpool:len:%d\n",pool->queue.len);
        }
		if(pool->id==2){
		printf("****************************************************\n");
		printf("pool_id:%d  \n1.readfilepool:wake_time=%f  sleep_time=%f\n",pool->id,wake_time,sleep_time);
		printf("2.readfilepool:max_working=%d min_working=%d \n",pool->max_working,pool->min_working);
		printf("3.readfilepool:len:%d\n",pool->queue.len);
        }
	    if(pool->id==3){
		printf("****************************************************\n");
		printf("pool_id:%d  \n1.sendmsgpool:wake_time=%f  sleep_time=%f\n",pool->id,wake_time,sleep_time);
		printf("2.sendmsgpool:max_working=%d min_working=%d \n",pool->max_working,pool->min_working);
		printf("3.sendmsgpool:len:%d\n",pool->queue.len);
		}


		pthread_mutex_lock(&(pool->thcount_lock));		
		pool->num_working--;
		if(pool->num_working==0)
		{
			pthread_cond_signal(&(pool->threads_all_idle));
		}
		pthread_mutex_unlock(&(pool->thcount_lock));
      }
	} 
	/*运行到此位置表明，线程将要退出，需更改当前线程池中的线程数量*/ 
	pthread_mutex_lock(&(pool->thcount_lock));//线程池锁，用于修改线程池中线程数量变量 
	pool->num_threads--;
	pthread_mutex_unlock(&(pool->thcount_lock));
	return NULL;   
}

struct {
    char *ext;
    char *filetype;
} extensions[] = {
        {"gif",  "image/gif"},
        {"jpg",  "image/jpg"},
        {"jpeg", "image/jpeg"},
        {"png",  "image/png"},
        {"ico",  "image/ico"},
        {"zip",  "image/zip"},
        {"gz",   "image/gz"},
        {"tar",  "image/tar"},
        {"htm",  "text/html"},
        {"html", "text/html"},
        {0,      0}};
typedef struct {
    int hit;
    int fd;
    char buffer[BUFSIZE + 1];
    int file_fd;
} webparam;

unsigned long get_file_size(const char *path) {
    unsigned long filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0) {
        return filesize;
    } else {
        filesize = statbuff.st_size;
    }
    return filesize;
}

void logger(int type, char *s1, char *s2, int socket_fd) {
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
}/* this is a web thread, so we can exit on errors */


void *web1(void *data)/*readmsg线程池*/ 
{
    int fd;
    int hit;
    int j, file_fd, buflen;
    long i, ret, len;
    char *fstr;
    char buffer[BUFSIZE + 1];
    webparam *param = (webparam *) data; 
    fd = param->fd;
    hit = param->hit;
    
    ret = read(fd, param->buffer, BUFSIZE); /* read web request in one go */
    if (ret == 0 || ret == -1) { /* read failure stop now */
        logger(FORBIDDEN, "failed to read browser request", "", fd);
    } else {
        if (ret > 0 && ret < BUFSIZE) /* return code is valid chars */
            param->buffer[ret] = 0;
/* terminate the buffer */
        else param->buffer[0] = 0;
        for (i = 0; i < ret; i++) /* remove cf and lf characters */
            if (param->buffer[i] == '\r' || param->buffer[i] == '\n')
                param->buffer[i] = '*';
        logger(LOG, "request", param->buffer, hit);
        if (strncmp(param->buffer, "GET ", 4) && strncmp(param->buffer, "GET ", 4)) {
            logger(FORBIDDEN, "only simple get operation supported", param->buffer, fd);
        }
        for (i = 4; i < BUFSIZE; i++) { /* null terminate after the second space to ignore extra stuff */
            if (param->buffer[i] == ' ') { /* string is "get url " +lots of other stuff */
                param->buffer[i] = 0;
                break;
            }
        }
        for (j = 0; j < i - 1; j++) /* check for illegal parent directory use .. */
            if (param->buffer[j] == '.' && param->buffer[j + 1] == '.') {
                logger(FORBIDDEN, "parent directory (..) path names not supported", param->buffer, fd);
            }
        if (!strncmp(&(param->buffer[0]), "GET /\0", 6) || !strncmp(&(param->buffer[0]), "GET /\0", 6)) /* convert no filename to
index file */
            (void) strcpy(param->buffer, "GET /index.html");
/* work out the file type and check we support it */
        buflen = strlen(param->buffer);
        fstr = (char *) 0;
        for (i = 0; extensions[i].ext != 0; i++) {
            len = strlen(extensions[i].ext);
            if (!strncmp(&(param->buffer[buflen - len]), extensions[i].ext, len)) {
                fstr = extensions[i].filetype;
                break;
            }
        }
        if (fstr == 0) logger(FORBIDDEN, "file extension type not supported", param->buffer, fd);
        
     task* curtask2=(task*)malloc(sizeof(struct task*));
     curtask2->next=NULL;
     curtask2->function=(void*)web2;//The first web; 
     curtask2->arg=(void*)param;
     addTask2ThreadPool(readfile_threadpool,curtask2);
}
}

void *web2(void *data) {
    int fd;
    int hit;
    int j, file_fd, buflen;
    long i, ret, len;
    char *fstr;
    char buffer[BUFSIZE + 1]; /* static so zero filled */
    webparam *param = (webparam *) data; 
    fd = param->fd;
    hit = param->hit;
    
     if ((param->file_fd = open(&(param->buffer[5]), O_RDONLY)) == -1) { /* open the file for reading */
            logger(NOTFOUND, "failed to open file", &(param->buffer[5]), fd);
        }
         logger(LOG, "send", &(param->buffer[5]), hit);
        len = (long) lseek(param->file_fd, (off_t) 0, SEEK_END);/* 使用 lseek 来获得文件长度,比较低效*/
        (void) lseek(param->file_fd, (off_t) 0, SEEK_SET);
        (void) sprintf(param->buffer,
                       "http/1.1 200 ok\nserver: nweb/%d.0\ncontent-length: %ld\nconnection:close\ncontent-type: %s\n\n",
                       VERSION, len, fstr);/* header + a blank line */
		logger(LOG, "header", param->buffer, hit);
        (void) write(fd, param->buffer, strlen(param->buffer));

    task* curtask3=(task*)malloc(sizeof(struct task*));
    curtask3->next=NULL;
    curtask3->function=(void*)web3;//The first web; 
    curtask3->arg=(void*)param;
    addTask2ThreadPool(sendmsg_threadpool,curtask3);
}

void *web3(void *data) {
	int fd;
    int hit;
    int j, file_fd, buflen;
    long i, ret, len;
    char *fstr;
    char buffer[BUFSIZE + 1]; /* static so zero filled */
    webparam *param = (webparam *) data; 
    fd = param->fd;
    hit = param->hit;
    /* send file in 8kb block - last block may be smaller */
    while ((ret = read(param->file_fd, param->buffer, BUFSIZE)) > 0) {
        (void) write(fd, param->buffer, ret);
    }

    usleep(10000);/*在 socket 通道关闭前,留出一段信息发送的时间*/
    close(param->file_fd);
    close(fd);
    //释放内存
    free(param);
}

int main(int argc, char **argv) {
    int i, port, pid, listenfd, socketfd, hit;
    socklen_t length;
	readmsg_threadpool=initThreadPool(30,1);
	readfile_threadpool=initThreadPool(30,2);
	sendmsg_threadpool=initThreadPool(100,3);
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
            task* curtask1=(task*)malloc(sizeof(struct task*));
            curtask1->next=NULL;
            curtask1->function=(void*)web1;//The first web; 
            curtask1->arg=(void*)param;
            addTask2ThreadPool(readmsg_threadpool,curtask1); 
    }
    destoryThreadPool(readmsg_threadpool);
    destoryThreadPool(readfile_threadpool);
    destoryThreadPool(sendmsg_threadpool);
}
