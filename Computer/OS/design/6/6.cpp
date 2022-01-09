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
    long length;//���ݳ��� 
    char *address;//������ʼ��ַ 
    int number;
	//�˱������ڼ�¼�ļ������ϣͰ��ʱ�䣬������Ϊȫ�ֱ�����
	//ÿ�������ļ�����Ͱ��ʱ����һ��¼�������丳ֵ���ļ��������ж��ļ��Ľ���ʱ�䡣 
} content;

typedef struct hashpair
{
    char *key;//�ļ��� 
    content *cont;//������ 
    struct hashpair *next;//��hashͰ�У�ָ����һ��hashpair 
    struct hashpair *prev;//��hashͰ�У�ָ����һ��hashpair 
} hashpair;

typedef struct hashtable
{
    hashpair **bucket;
    int num_bucket;//��ϣͰ�ĸ��� 
    int *capacity;//ÿ����ϣͰ�ĸ��� 
	volatile int* locks;
} hashtable;

static inline long int hashString(char *str)//�ַ���ת��Ϊ��ϣ�� 
{
    int hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    while(hash < 0 || hash >= HASHCOUNT)
        hash = (hash + HASHCOUNT) % HASHCOUNT;//��ֹ��ϣֵ���󣬽���ȡ�� 
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

//�ж�����content�Ƿ���ͬ����ͬ����1����ͬ����0 
static inline int isEqualContent(content *cont1, content *cont2)
{
    if (cont1->length != cont2->length)
        return 0;
    if (cont1->address != cont2->address)
        return 0;
    return 1;
}

int frame_fault;//ȫ�ֱ�����ȱҳ���� 
int sum;//ȫ�ֱ������ܷ������� 
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
    memset(table->bucket, 0, buckets * sizeof(void *));//��0 
    table->num_bucket = buckets;
    //��ʼ�����ź�
    table->locks = (int *)malloc(buckets * sizeof(int));
    if (!table->locks)
    {
        free(table);
        return NULL;
    }
    memset((int *)&table->locks[0], 0, buckets * sizeof(int));
    table->capacity = (int *)malloc(buckets * sizeof(int));//��ϣ��Ĵ�С 
    int i;
    for ( i = 0; i < buckets; i++)
        table->capacity[i] = SIZE;//ÿ����ϣͰ��С����ʼֵ��Ϊ40 
    return table;
}

//�ͷ�hash table�е���Դ 
void freeHashTable(hashtable *table)
{
    if (table == NULL)
    {
        return;
    }
    hashpair *next;
    int i;
    for (i = 0; i < table->num_bucket; i++)
    	//���Ͱ�ͷ� 
    {
        hashpair *pair = table->bucket[i];//ÿ��Ͱ�ڽ�������ͷ� 
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
    int hash = hashString(key) % table->num_bucket;//���key��hash���е�λ�� 
    // get entry
    hashpair *pair = table->bucket[hash];//�ҵ���ӦhashͰ�е��׸�pair 
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
            //�����滻 
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
    //δ�ҵ���Ӧkeyֵ�����в������ 
    pair = (hashpair *)malloc(sizeof(hashpair));
    pair->key = copystring(key);
    pair->cont = cont;
    pair->next = table->bucket[hash];//�����ײ�  ���� 
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
    
    while (pair) //���Ͱ�д��ڵ��ļ� �ҵ��������һ�����ڵ� 
	{
        prev = pair;
        pair = pair->next;
    }
    if(prev)//�����������һ���ļ� 
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

//����keyֵ����hash���в�����Ӧ�� 
content *getContentByKey(hashtable *table, char *key)
{
    int hash = hashString(key) % table->num_bucket;
    hashpair *pair = table->bucket[hash];
    while (pair)
    {
        if (0 == strcmp(pair->key, key))
        {
            if (pair->prev != NULL && pair->next != NULL)//λ�������м�λ�� ���ƶ�����ӦͰ������ͷ 
            {
                pair->prev->next = pair->next;
                pair->next->prev = pair->prev;
                table->bucket[hash]->prev = pair;
                pair->next = table->bucket[hash];
                pair->prev = NULL;
                table->bucket[hash] = pair;
            }
            else if(pair->prev != NULL && !pair->next)//λ������β��λ�� ���ƶ�����ӦͰ������ͷ 
            {
                pair->prev->next = pair->next;
                table->bucket[hash]->prev = pair;
                pair->next = table->bucket[hash];
                pair->prev = NULL;
                table->bucket[hash] = pair;
            }

            return pair->cont;//���ҳɹ� 
        }

        pair = pair->next;
    }
    return NULL;//����ʧ�� 
}

/*queue status and conditional variable*/ 
typedef struct staconv {
    pthread_mutex_t mutex;
    pthread_cond_t cond;/*���������ͻ����̳߳����߳�*/ 
    bool status;/*��ʾ�������״̬*/ 
} staconv;

/*Task*/
typedef struct task {
    struct task *next;/*ָ����һ����*/ 
    void (*function)(void *arg);/*����ָ��*/ 
    void *arg;/*��������ָ��*/ 
} task; 

/*Task Queue*/
typedef struct taskqueue {
    pthread_mutex_t mutex;/*���ڻ����/д�������*/ 
    task *front;/*ָ�����*/ 
    task *rear;/* ָ���β*/ 
    staconv *has_jobs;/*����״̬�������߳�*/ 
    int len;/*�������������*/ 
} taskqueue;  

/*Thread*/
typedef struct thread {
    int id;/*�߳�id*/ 
    pthread_t pthread;/*��װ��POSIX�߳�*/ 
    struct threadpool *pool;/*���̳߳ذ�*/ 
} thread;

  
typedef struct threadpool {
    thread **threads;/*�߳�ָ������*/ 
    volatile int num_threads;/*�̳߳����߳�����*/ 
    volatile int num_working;/*Ŀǰ���ڹ������̸߳���*/ 
    pthread_mutex_t thcount_lock;/*�̳߳��������޸�������������*/ 
    pthread_cond_t threads_all_idle;/*���������̵߳���������*/ 
    taskqueue queue;/*�������*/ 
    volatile bool is_alive;/*��ʾ�̳߳��Ƿ񻹴��*/ 
} threadpool;
  
int create_thread(struct threadpool* pool,struct thread* pthread,int id);
void* thread_do(struct thread* pthread);

/*��ʼ���������*/
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


/*�̳߳س�ʼ������*/ 
struct threadpool* initThreadPool(int num_threads) {
	//�����߳̿ռ� 
   	threadpool* pool;
	pool=(threadpool*)malloc(sizeof(struct threadpool));
	pool->num_threads=0;
	pool->num_working=0;
	//��ʼ������������������ 
	pthread_mutex_init(&(pool->thcount_lock),NULL);
	pthread_cond_init(&(pool->threads_all_idle),NULL);
	pool->is_alive=TRUE;
    //��ʼ��������� 
	init_taskqueue(&(pool->queue));
    //�����߳����� 
	pool->threads=(struct thread**)malloc(num_threads * sizeof(struct thread*));
	//�����߳� 
	int i;
	for(i=0;i<num_threads;i++)
	{
		create_thread(pool,pool->threads[i],i);//iΪ�߳�id 
	}
	while(pool->num_threads!=num_threads){}
	return pool;
}

 /*������������*/
void push_taskqueue(taskqueue* queue,task* curtask) //������� ��ǰ���� 
{
	pthread_mutex_lock(&(queue->mutex));//���ڻ����/д�������
	if(queue->len==0)//���������������Ϊ0
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
	pthread_cond_signal(&(queue->has_jobs->cond));//���ڻ����̳߳��е��߳�
	pthread_mutex_unlock(&(queue->has_jobs->mutex));
	pthread_mutex_unlock(&(queue->mutex));
}

/*���̳߳����������*/ 
void addTask2ThreadPool(threadpool* pool, task* curtask) {
    push_taskqueue(&pool->queue, curtask);
}

/*�ȴ���ǰ����ȫ��������*/ 
void waitThreadPool(threadpool* pool) {
    pthread_mutex_lock(&pool->thcount_lock);
    while (pool->queue.len || pool->num_working) {
        pthread_cond_wait(&pool->threads_all_idle, &pool->thcount_lock);
    }
    pthread_mutex_unlock(&pool->thcount_lock);
}

/*�����������*/
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

/*�����̳߳�*/ 
void destoryThreadPool(threadpool* pool){
//�����ǰ�����������������ȴ��������Ϊ�գ����������߳�ִ���������
    
	pthread_mutex_lock(&pool->thcount_lock);
    waitThreadPool(pool);//�ȴ���ǰ����ȫ��������
    destory_taskqueue(&pool->queue);//�����������
//�����߳�ָ�����飬���ͷ�����Ϊ�̳߳ط�����ڴ�
    int i;
    for (i = 0; i < pool->num_threads; ++i)
        free(pool->threads[i]);
    free(pool->threads);
    pool->is_alive = FALSE;
	pthread_mutex_unlock(&pool->thcount_lock);
    pthread_mutex_destroy(&(pool-> thcount_lock));//������
    pthread_cond_destroy(&(pool->threads_all_idle));//������������
}

/*��õ�ǰ�̳߳������������̵߳�����*/ 
int getNumofThreadWorking(threadpool* pool) {
    return pool->num_working;
}


/*���������ͷ����ȡ���񣬲��ڶ�����ɾ��������*/ 
task* take_taskqueue(taskqueue* queue)
{
	pthread_mutex_lock(&queue->mutex);
	if (queue->len != 0)//�����д������� 
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
			pthread_cond_signal(&queue->has_jobs->cond);//�����߳� 
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
/*�����߳�*/ 
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

/*�߳����е��߼�����*/ 
void* thread_do(struct thread* pthread){
	/*�����߳�����*/
	char thread_name[128]={0};
	sprintf(thread_name,"thread-poop-%d",pthread->id);
	
	prctl(PR_SET_NAME,thread_name);
	
	/*����̳߳�*/
	threadpool* pool=pthread->pool;
	
	/*���̳߳س�ʼ��ʱ�������Ѿ������̵߳ļ�����ִ��pool->num_thread++*/
	
	pthread_mutex_lock(&(pool->thcount_lock));//�̳߳����������޸��̳߳����߳��������� 
	pool->num_threads++;
	pthread_mutex_unlock(&(pool->thcount_lock));
	/*�߳�һֱѭ���������У�ֱ��pool->is_alive��Ϊfalse*/
	while(pool->is_alive){

		/*�����������л���������������У���������*/
		pthread_mutex_lock (&(pool->queue.has_jobs->mutex));//���ڻ����д������� 
		 while ((pool->queue.has_jobs->status == 0) &&pool->is_alive)//������״̬ 
         {
             pthread_cond_wait (&(pool->queue.has_jobs->cond), &(pool->queue.has_jobs->mutex));//���� 
         }
		pthread_mutex_unlock (&(pool->queue.has_jobs->mutex));
		
		if(pool->is_alive){ 
		/*ִ�е���λ�ã������߳��ڹ�������Ҫ�Թ����߳��������м���*/
		//pool->num_working++
		pthread_mutex_lock (&(pool->thcount_lock));//�̳߳����������޸��̳߳������ڹ����߳��������� 
        pool->num_working++;
        pthread_mutex_unlock (&(pool->thcount_lock));

		/*��������еĶ�����ȡ����ִ��*/
		  void(*func)(void*);
		  void* arg;
		  //take_taskqueue���������ͷ����ȡ���񣬲��ڶ�����ɾ��������
		   task* curtask=take_taskqueue(&pool->queue);
			if(curtask)
			{
				func=curtask->function;
				arg=curtask->arg;
				//ִ������ 
				func(arg);
				//�ͷ����� 
				free(curtask);
			}
			/* ִ�е���λ�ã������߳��Ѿ�������ִ����ɣ���Ҫ�Ĺ����߳�����*/
			/*�˴�����ע�⣬�������߳�����Ϊ0����ʾ����ȫ����ɣ�Ҫ��������waitThreadPool()�����ϵ��̼߳�������*/
			pthread_mutex_lock (&(pool->thcount_lock));//�̳߳����������޸��̳߳������ڹ����߳��������� 
    		pool->num_working--;
			pthread_mutex_unlock(&(pool->thcount_lock));
			if(pool->num_working == 0)//����ȫ����� 
				pthread_cond_signal(&(pool->threads_all_idle));//�����߳� 
		} 
	}
	/*���е���λ�ñ������߳̽�Ҫ�˳�������ĵ�ǰ�̳߳��е��߳�����*/ 
	//pool->num_threads--;
	pthread_mutex_lock(&(pool->thcount_lock));//�̳߳����������޸��̳߳����߳��������� 
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
/*�˺��������webserver��Ҫ���ܣ������Ƚ����ͻ��˷��͵���Ϣ��Ȼ����л�ȡ�ͻ���������ļ�����
Ȼ������ļ����ӱ��ؽ����ļ����뻺�棬��������Ӧ��HTTP��Ӧ��Ϣ�����ͨ����������ͻ��˵�socket
ͨ����ͻ��˷���HTTP��Ӧ��Ϣ*/ 
void web(void *data) {

    int fd;
    int hit;
    int j, file_fd, buflen;
    long i, ret, len;
    char *fstr;
    char buffer[BUFSIZE + 1]; /* static so zero filled *//*���þ�̬������*/ 
    webparam *param = (webparam *) data;
    fd = param->fd;
    hit = param->hit;
    
    ret = read(fd, buffer, BUFSIZE); /* read web request in one go *//*������ͨ���ж�ȡ�ͻ��˵�������Ϣ*/ 
    if (ret == 0 || ret == -1) 
	{ /* read failure stop now */
	  /*�����ȡ�ͻ�����Ϣʧ�ܣ�����ͻ��˷���HTTPʧ����Ӧ��Ϣ*/ 
        logger(FORBIDDEN, "failed to read browser request", "", fd);
    } 
	else 
	{
        if (ret > 0 && ret < BUFSIZE) /* return code is valid chars *//*������Ч�ַ����������ַ���β����ʾΪ0*/ 
            buffer[ret] = 0;/* terminate the buffer */
        else buffer[0] = 0;
        for (i = 0; i < ret; i++) /* remove cf and lf characters */
            if (buffer[i] == '\r' || buffer[i] == '\n')
                buffer[i] = '*';
        logger(LOG, "request", buffer, hit);
        /*�жϿͻ���HTTP������Ϣ�Ƿ�ΪGET���ͣ���������������Ӧ����Ӧ��Ϣ*/ 
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
        /*���������Ϣ��û�а�����Ч���ļ�������ʹ��Ĭ���ļ���index.html*/ 
            (void) strcpy(buffer, "GET /index.html");
        /* work out the file type and check we support it */
        /*�����������������Ƿ�֧��*/ 
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
        
        content *cont = getContentByKey(table, &buffer[5]);  //�ж��Ƿ��ڹ�ϣ����
        if (cont == NULL)//������� 
		{
			content *contprecent = (content *)malloc(sizeof(content));
			if (contprecent == NULL)
                free(temp);
            else
            {
            	if ((file_fd = open(&buffer[5], O_RDONLY)) == -1) 
				{ /* open the file for reading */
				/*��ָ���ļ���*/ 
                   logger(NOTFOUND, "failed to open file", &buffer[5], fd);
                }
                logger(LOG, "send", &buffer[5], hit);
                len = (long) lseek(file_fd, (off_t) 0, SEEK_END); /* ʹ�� lseek ������ļ�����,�Ƚϵ�Ч*/
                (void) lseek(file_fd, (off_t) 0, SEEK_SET);
                (void) sprintf(buffer,
                       "http/1.1 200 ok\nserver: nweb/%d.0\ncontent-length: %ld\nconnection:close\ncontent-type: %s\n\n",
                       VERSION, len, fstr); /* header + a blank line */
		        logger(LOG, "header", buffer, hit);
                (void) write(fd, buffer, strlen(buffer));
                
                char *filetext = malloc(sizeof(char) * (len + 1));
                if (table->capacity[hashString(&temp[5]) % HASHCOUNT] == 0)//�����ϣͰ��ʣ��Ϊ�գ���ɾ��һ�������� 
                {
                	deleteTail(table, &temp[5]); 
				}
				if (len > 0) //��� 
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
                    addItem(table, &temp[5], contprecent);  //��������������ӵ���ϣ����
                }
                 close(file_fd);

			}


		}
		else //����Ѿ��ڹ�ϣ���У�ֱ�Ӷ�ȡ��ҳ����
		{
			pthread_mutex_lock(&mutex_frame);
			sum++;
			printf("sum= %d    frame_fault= %d      fault%= %f\n",sum,frame_fault,(1.0*frame_fault)/sum);
			num++;
			cont->number=num; //����ʱ�� 
			pthread_mutex_unlock(&mutex_frame);
			logger(LOG, "send", &buffer[5], hit);                                                                                                                   
            (void)sprintf(buffer, "HTTP/1.1 200 OK\nServer: nweb/%d.0\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", VERSION, cont->length, fstr); 
             logger(LOG, "header", buffer, hit);
            (void)write(fd, buffer, strlen(buffer));
			(void)write(fd, cont->address, cont->length);
		}

        sleep(0.00001);/*�� socket ͨ���ر�ǰ,����һ����Ϣ���͵�ʱ��*/
        close(file_fd);
    }
    close(fd);
//�ͷ��ڴ�
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
    
    pool=initThreadPool(40);//�����̳߳����̸߳���Ϊ40
    logger(LOG, "nweb starting", argv[1], getpid());
/* setup the network socket */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        logger(ERROR, "system call", "socket", 0);
    port = atoi(argv[1]);
    if (port < 0 || port > 60000)logger(ERROR, "Invalid port number (try 1->60000)", argv[1], 0);
//��ʼ���߳�����,Ϊ����״̬
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
		    curtask->function=web;//����ָ�� 
		    curtask->arg=(void*)param;//��������ָ�� 
	     	addTask2ThreadPool(pool,curtask);  
    }
    destoryThreadPool(pool);
    freeHashTable(table);
}
