//编译代码指令 gcc -std=gnu99 -g -o multithread_webserver
// multithread_webserver.c -lphtread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/wait.h>
#include <wait.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <semaphore.h>
#include <sys/mman.h> // shared memory
#include <sys/time.h>

#include "cut.h"

#define SEM_NAME "sem_count"
#define SHM_NAME "mmap_example"
struct timeval start, end;
double timeuse = 0, time_totol = 0;
sem_t *psem;
int shm_fd;
void *memPtr;

#define VERSION 23
#define BUFSIZE 8096
#define ERROR 42
#define LOG 44
#define FORBIDDEN 403
#define NOTFOUND 404

#define FILEBUFSIZE 1024 * 1024 // 读文件的缓存区，1MB 大小

#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif

threadpool *read_msg_pool;
threadpool *read_file_pool;
threadpool *send_msg_pool;

struct {
  char *ext;
  char *filetype;
} extensions[] = {{"gif", "image/gif"},
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

typedef struct {
  int hit;
  int fd;
  char *fstr;               // for file type
  char buffer[BUFSIZE + 1]; /* static so zero filled */
  char file_buffer[FILEBUFSIZE];
  int ret;
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
  // s1 is request:, s2 is GET /
  int fd;
  char logbuffer[BUFSIZE * 2];
  switch (type) {
  case ERROR:
    (void)sprintf(logbuffer, "ERROR: %s:%s Errno=%d exiting pid=%d", s1, s2,
                  errno, getpid());
    break;
  case FORBIDDEN:
    (void)write(socket_fd,
                "HTTP/1.1 403 Forbidden\nContent-Length: 185\nConnection: "
                "close\nContent-Type: text/html\n\n<html><head>\n<title>403 "
                "Forbidden</title>\n</head><body>\n<h1>Forbidden</h1>\nThe "
                "requested URL, file type or operation is not allowed on this "
                "simple static file webserver.\n</body></html>\n",
                271);
    (void)sprintf(logbuffer, "FORBIDDEN: %s:%s", s1, s2);
    break;
  case NOTFOUND:
    (void)write(socket_fd,
                "HTTP/1.1 404 Not Found\nContent-Length: 136\nConnection: "
                "close\nContent-Type: text/html\n\n<html><head>\n<title>404 "
                "Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe "
                "requested URL was not found on this server.\n</body></html>\n",
                224);
    (void)sprintf(logbuffer, "NOT FOUND: %s:%s", s1, s2);
    break;
  case LOG:
    (void)sprintf(logbuffer, " INFO: %s:%s:%d", s1, s2, socket_fd);
    break;
  }
  /* No checks here, nothing can be done with a failure anyway */
  if ((fd = open("nweb.log", O_CREAT | O_WRONLY | O_APPEND, 0644)) >= 0) {
    (void)write(fd, logbuffer, strlen(logbuffer));
    (void)write(fd, "\n", 1);
    (void)close(fd);
  }
  // if(type == ERROR || type == NOTFOUND || type == FORBIDDEN) exit(3);
}

void *SendMsgThreadPool(void *data);
void *ReadFileThreadPool(void *data);

void *ReadMsgThreadPool(void *data) {
  gettimeofday(&start, NULL);
  webparam *param = (webparam *)data;
  int fd = param->fd;
  char *buffer = param->buffer;
  int hit = param->hit;
  long i, len;
  int j;
  int ret = read(fd, buffer, BUFSIZE);
  if (ret == 0 || ret == -1) { /* read failure stop now */
    logger(FORBIDDEN, "failed to read browser request", "", fd);
    close(fd);
    return data; // void * 返回任意类型的指针
  } else {
    if (ret > 0 && ret < BUFSIZE) /* return code is valid chars */
      buffer[ret] = 0;
    /* terminate the buffer */
    else
      buffer[0] = 0;
    for (i = 0; i < ret; i++) /* remove cf and lf characters */
      if (buffer[i] == '\r' || buffer[i] == '\n')
        buffer[i] = '*';
    logger(LOG, "request", buffer, hit);
    if (strncmp(buffer, "GET ", 4) &&
        strncmp(buffer, "get ", 4)) { // GET 从何而来，socket
      logger(FORBIDDEN, "only simple get operation supported", buffer, fd);
    }
    for (i = 4; i < BUFSIZE; i++) { /* null terminate after the second space to
                                       ignore extra stuff */
      if (buffer[i] == ' ') { /* string is "get url " +lots of other stuff */
        buffer[i] = 0;
        break;
      }
    }

    for (j = 0; j < i - 1; j++) /* check for illegal parent directory use .. */
      if (buffer[j] == '.' && buffer[j + 1] == '.') {
        logger(FORBIDDEN, "parent directory (..) path names not supported",
               buffer, fd);
      }
    if (!strncmp(&buffer[0], "GET /\0", 6) ||
        !strncmp(&buffer[0], "GET /\0", 6)) /* convert no filename to
  index file */
      (void)strcpy(buffer, "GET /index.html");
    /* work out the file type and check we support it */
    int buflen = strlen(buffer);
    param->fstr = (char *)0;
    for (i = 0; extensions[i].ext != 0; i++) {
      len = strlen(extensions[i].ext);
      if (!strncmp(&buffer[buflen - len], extensions[i].ext, len)) {
        param->fstr = extensions[i].filetype;
        break;
      }
    }
    if (param->fstr == 0)
      logger(FORBIDDEN, "file extension type not supported", buffer, fd);
  }
  gettimeofday(&end, NULL);

  double timeuse = (end.tv_sec - start.tv_sec) +
                   (double)(end.tv_usec - start.tv_usec) / 1000000.0;
  /*printf("平均每个客户端完成读 socket 时间为 %fms\n", timeuse * 1000);*/
  sem_wait(psem);
  *((double *)memPtr) += timeuse * 1000;
  sem_post(psem);

  /*readFileThreadPool(param);*/
  task *read_file_task = (task *)malloc(sizeof(task));
  read_file_task->arg = (void *)param;
  read_file_task->function = ReadFileThreadPool;
  read_file_task->next = NULL;
  addTask2ThreadPool(read_file_pool, read_file_task);
  return param;
}

void *ReadFileThreadPool(void *data) {
  gettimeofday(&start, NULL);
  webparam *param = (webparam *)data;
  char *buffer = param->buffer;
  int fd = param->fd;
  int hit = param->hit;
  int file_fd;
  long len;
  if ((file_fd = open(&buffer[5], O_RDONLY)) ==
      -1) { /* open the file for reading */
    logger(NOTFOUND, "failed to open file", &buffer[5], fd);
  }

  logger(LOG, "send", &buffer[5], hit);
  len = (long)lseek(file_fd, (off_t)0,
                    SEEK_END); /* 使用 lseek 来获得文件⻓度,比较低效*/
  (void)lseek(file_fd, (off_t)0, SEEK_SET);
  /* 想想还有什么方法来获取*/
  (void)sprintf(buffer,
                "http/1.1 200 ok\nserver: nweb/%d.0\ncontent-length: "
                "%ld\nconnection: close\ncontent-type: %s\n\n",
                VERSION, len, param->fstr); /* header + a blank line */
  logger(LOG, "header", buffer, hit);
  (void)write(fd, buffer, strlen(buffer));
  param->ret = read(file_fd, param->file_buffer, FILEBUFSIZE); // > 0, success
  close(file_fd);

  gettimeofday(&end, NULL);
  timeuse = end.tv_sec - start.tv_sec +
            (double)(end.tv_usec - start.tv_usec) / 1000000.0;
  /*printf("平均每个客户端完成写日志数据时间为 %fms\n", timeuse * 1000);*/
  sem_wait(psem);
  *((double *)memPtr + 1) += timeuse * 1000;
  sem_post(psem);

  /*SendMsgThreadPool(param);*/
  task *send_msg_task = (task *)malloc(sizeof(task));
  send_msg_task->arg = (void *)param;
  send_msg_task->function = SendMsgThreadPool;
  send_msg_task->next = NULL;
  addTask2ThreadPool(send_msg_pool, send_msg_task);
  return param;
}

void *SendMsgThreadPool(void *data) {
  gettimeofday(&start, NULL);
  webparam *param = (webparam *)data;
  char *file_buffer = param->file_buffer;
  int fd = param->fd;
  (void)write(fd, file_buffer, param->ret);

  gettimeofday(&end, NULL);
  timeuse = end.tv_sec - start.tv_sec +
            (double)(end.tv_usec - start.tv_usec) / 1000000.0;
  /*printf("平均每个客户端完成写 socket 的时间为 %fms\n", timeuse * 1000);*/
  sem_wait(psem);
  *((double *)memPtr + 2) += timeuse * 1000;
  sem_post(psem);

  char buffer[BUFSIZE + 1]; /* static so zero filled */
  (void)sprintf(buffer,
                "平均 readmsg 时间为 %fms\n "
                "平均 readfile 时间为 %fms\n "
                "平均 sendmsg 时间为 %fms\n",
                *((double *)memPtr) / param->hit,
                *((double *)memPtr + 1) / param->hit,
                *((double *)memPtr + 2) / param->hit); /* header + a blank line
                                                        */
  logger(LOG, "time", buffer, param->hit);

  /*usleep(10000); [>在 socket 通道关闭前,留出一段信息发送的时间<]*/
  close(fd);
  free(param);
}

int main(int argc, char **argv) {

  int i, port, pid, listenfd, socketfd, hit;
  socklen_t length;
  static struct sockaddr_in cli_addr;  /* static = initialised to zeros */
  static struct sockaddr_in serv_addr; /* static = initialised to zeros */
  if (argc < 3 || argc > 3 || !strcmp(argv[1], "-?")) {
    (void)printf(
        "hint: nweb Port-Number Top-Directory\t\tversion %d\n\n"
        "\tnweb is a small and very safe mini web server\n"
        "\tnweb only servers out file/web pages with extensions named below\n"
        "\t and only from the named directory or its sub-directories.\n"
        "\tThere is no fancy features = safe and secure.\n\n"
        "\tExample: nweb 8181 /home/nwebdir &\n\n"
        "\tOnly Supports:",
        VERSION);
    for (i = 0; extensions[i].ext != 0; i++)
      (void)printf(" %s", extensions[i].ext);
    (void)printf(
        "\n\tNot Supported: URLs including \"..\", Java, Javascript, CGI\n"
        "\tNot Supported: directories / /etc /bin /lib /tmp /usr /dev /sbin \n"
        "\tNo warranty given or implied\n\tNigel Griffiths nag@uk.ibm.com\n");
    exit(0);
  }
  if (!strncmp(argv[2], "/", 2) || !strncmp(argv[2], "/etc", 5) ||
      !strncmp(argv[2], "/bin", 5) || !strncmp(argv[2], "/lib", 5) ||
      !strncmp(argv[2], "/tmp", 5) || !strncmp(argv[2], "/usr", 5) ||
      !strncmp(argv[2], "/dev", 5) || !strncmp(argv[2], "/sbin", 6)) {
    (void)printf("ERROR: Bad top directory %s, see nweb -?\n", argv[2]);
    exit(3);
  }
  if (chdir(argv[2]) == -1) {
    (void)printf("ERROR: Can't Change to directory %s\n", argv[2]);
    exit(4);
  }

  /* Become deamon + unstopable and no zombies children (= no wait()) */
  /*if (fork() != 0)*/
  /*return 0; [> parent returns OK to shell <]*/

  /*(void)signal(SIGCLD, SIG_IGN); [> ignore child death <]*/
  /*(void)signal(SIGHUP, SIG_IGN); [> ignore terminal hangups <]*/
  /*for (i = 0; i < 32; i++)       // what meaning?*/
  /*(void)close(i);*/
  /* close open files */

  // 设置组的 pid 为 点前进程的 pid
  /*(void)setpgrp(); [> break away from process group <]*/

  logger(LOG, "nweb starting", argv[1], getpid());
  /* setup the network socket */
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    logger(ERROR, "system call", "socket", 0);
  port = atoi(argv[1]);
  if (port < 0 || port > 60000)
    logger(ERROR, "Invalid port number (try 1->60000)", argv[1], 0);

  //初始化线程属性,为分离状态
  /*pthread_attr_t attr;*/
  /*pthread_attr_init(&attr);*/
  /*pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);*/

  /*pthread_t pth;*/

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    logger(ERROR, "system call", "bind", 0);
  if (listen(listenfd, 64) < 0)
    logger(ERROR, "system call", "listen", 0);

  if ((psem = sem_open(SEM_NAME, O_CREAT, 0777, 1)) ==
      SEM_FAILED) { // 信号量是否为全局变量，fork 会咋样？
    perror("create semaphore error");
    exit(1);
  }

  if ((shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0777)) < 0) {
    perror("create shared memory object error");
    exit(1);
  }
  ftruncate(shm_fd, 3 * sizeof(double));
  memPtr = mmap(NULL, 3 * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED,
                shm_fd, 0);
  if (memPtr == MAP_FAILED) {
    perror("create mmap error");
    exit(1);
  }

  *(double *)memPtr = 0;
  *((double *)memPtr + 1) = 0;
  *((double *)memPtr + 2) = 0;

  read_msg_pool = initThreadPool(15);
  read_file_pool = initThreadPool(15);
  send_msg_pool = initThreadPool(15);
  for (hit = 1;; hit++) { // accept and create pthread
    length = sizeof(cli_addr);
    if ((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) <
        0)
      logger(ERROR, "system call", "accept", 0);
    webparam *param = (webparam *)malloc(sizeof(webparam));
    param->hit = hit;
    param->fd = socketfd;
    task *read_msg_task = (task *)malloc(sizeof(task));
    read_msg_task->arg = (void *)param;
    read_msg_task->function = ReadMsgThreadPool;
    read_msg_task->next = NULL;

    addTask2ThreadPool(read_msg_pool, read_msg_task);
  }
}
