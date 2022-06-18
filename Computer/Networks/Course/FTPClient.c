// Time:2022-06-15 17:02:09
#include <stdio.h>

// for memset
#include <string.h>
// for exit
#include <stdlib.h>
// 是Unix/Linux系统的基本系统数据类型的头文件，含有一些数据类型的定义。
#include <sys/types.h>
// 提供socket函数及数据结构
#include <netinet/in.h>
#include <sys/socket.h>
// use for hostent
#include <arpa/inet.h>
#include <netdb.h>
// for read/write
#include <unistd.h>
// for file
#include <fcntl.h>
// for getting file size using stat()
#include <sys/stat.h>
// for sendfile()
#include <sys/sendfile.h>

/*
 * 默认情况下FTP协议使用TCP端口中的
 * 20和21这两个端口，其中20用于传输数据，21用于传输控制信息。
 *
 * 一个是控制通道, 一个是数据通道。
 * 控制通道是和 FTP 服务器进行沟通的通道, 先由客户端发送 TCP 连接服务器。
 * 数据通道则是和 FTP 服务器进行文件传输或者获取文件列表的通道。
 *
 * 主动方式(PORT 方式)和被动方式(PASV方式)。
 *
 * 主动方式下, FTP 客户端首先和 FTP 服务器的控制通道对应端口(一般为21)建立连接,
 * 通过控制通道发送命令,
 * 客户端需要接收数据的时候在这个通道上发送 PORT 命令。
 * PORT 命令包含了客户端用什么端口(一个大于 1024 的端口)接收数据。
 * 在传送数据的时候，服务器端通过自己的 TCP 20
 * 端口连接至客户端的指定端口发送数据。
 * 在传输数据的时候,FTP 服务器必须和客户端建立一个新的连接。
 *
 * 被动方式下, 建立控制通道的过程和主动方式类似, 当客户端通过这个通道发送 PASV
 * 命令的时候, FTP Server 打开一个位于 1024~5000 之间的随机端口并且通知客户端,
 * 然后客户端与服务器之间将通过这个端口进行数据的传送。
 * 这个时候FTP server不再需要建立一个新的和客户端之间的连接，
 * 而是客户端需要建立一个新的连接来与服务器进行数据传输。
 */

#define read_len 256
char command[read_len];

void command_help();
void command_port(int *, char *);
void command_pasv(int *, char *);
void command_ls(int *, int *, char *);
void command_get(int *, int *, char *);
void command_put(int *, int *, char *);

int login(int *control_sock) {
  printf("Please login in!\n");
  /* 命令 ”USER username\r\n” */
  char username[24];
  printf("Please input username: ");
  scanf("%s", username);
  bzero(command, strlen(command));
  /*sprintf(command, "USER %s\r\n", username);*/
  sprintf(command, "USER %s", username);
  write(*control_sock, command, strlen(command));
  /* 接收服务器的响应码和信息，正常 "331 User name okay, need password." */
  bzero(command, strlen(command));
  read(*control_sock, command, read_len);
  printf("%s", command);
  if (strstr(command, "331") == NULL) {
    printf("Login Error!\n");
    return -1;
  }

  /* 命令 ”PASS password\r\n” */
  char *password;
  password = getpass("Please input password: ");
  bzero(command, strlen(command));
  /*sprintf(command, "PASS %s\r\n", password);*/
  sprintf(command, "PASS %s", password);
  /* 客户端发送密码到服务器端 */
  write(*control_sock, command, strlen(command));
  /* 客户端接收服务器的响应码和信息，正常为 ”230 User logged in, proceed.” */
  bzero(command, strlen(command));
  read(*control_sock, command, read_len);
  printf("%s", command);
  getchar();
  if (strstr(command, "230") == NULL) {
    printf("Login Error!\n");
    return -1;
  }
  return 0;
}

// 客户端与服务器的命令通道和数据通道需要分离
/*int main(int argc, char *argv[]) {*/
int main(void) {
  // 当客户端与服务器建立连接后，服务器会返回 220 的响应码和一些欢迎信息。
  int control_sock;

  // get ip of server
  /*struct hostent *hp;*/
  /*hp = gethostbyname(argv[1]);*/
  // define socket
  struct sockaddr_in server_addr;
  char server_ip[16];
  int server_port;
  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  /* 初始化控制socket */
  if ((control_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Sokcet creates Error!\n");
    return -1;
  }
  /*memcpy(&server_addr.sin_addr, hp->h_addr, hp->h_length);*/
  server_addr.sin_family = AF_INET;
  printf("Please input the ip address you intend to connect: ");
  scanf("%s", server_ip);
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  printf("Please input the port you intend to connect: ");
  scanf("%d", &server_port);
  // 换行符
  getchar();
  // The htons() function converts the unsigned short integer hostshort from
  // host byte order to network byte order. 转换成网络字节
  server_addr.sin_port = htons(server_port);

  /* 连接到服务器端 */
  if ((connect(control_sock, (struct sockaddr *)&server_addr,
               sizeof(server_addr))) < 0) {
    printf("Connect Error!\n");
    return -1;
  }
  bzero(command, read_len);
  /* 客户端接收服务器端的一些欢迎信息 */
  read(control_sock, command, read_len);
  printf("%s", command);

  // login
  if (login(&control_sock) == -1) {
    return -1;
  }

  // 命令通道
  while (1) {
    printf("ftp> ");
    bzero(command, strlen(command));
    // fgets函数从stdin流中读取 read_len - 1 个字符放入command中
    if (fgets(command, read_len, stdin) == NULL) {
      printf("Input Error!\n");
      break;
    }

    command[strlen(command) - 1] =
        '\0'; // fgets函数读取的最后一个字符为换行符，此处将其替换为'\0'

    printf("Input Command is [%s]\n", command);

    // 命令匹配可改进
    if (strncmp(command, "?", 1) == 0) //比较两个字符串前4个字节，若相等则返回0
    {
      command_help();
      continue;
    } else if (strncmp(command, "quit", 4) == 0) {
      /* 客户端接收服务器的响应码，正常为 ”200 Closes connection.” */
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        continue;
      }
      bzero(command, strlen(command));
      read(control_sock, command, read_len);
      printf("%s", command);
      close(control_sock);
      printf("Bye!\n");
      exit(0); //结束进程
    } else if (strncmp(command, "pwd", 3) == 0) {
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        continue;
      }
      bzero(command, strlen(command));
      read(control_sock, command, read_len);
      printf("The path of the remote directory is: %s\n", command);
      continue;
    } else if (strncmp(command, "rm", 2) == 0) {
      write(control_sock, command, strlen(command));
      bzero(command, strlen(command));
      read(control_sock, command, read_len);
      printf("%s", command);
      continue;
    } else if (strncmp(command, "cd", 2) == 0) {
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        printf("Remote directory failed to change!\n");
        continue;
      } else {
        /* 客户端接收服务器的响应码和信息，正常为 ”250 Command okay.” */
        printf("Remote directory successfully changed\n");
        bzero(command, strlen(command));
        read(control_sock, command, read_len);
        printf("Now the path of the remote directory is: %s\n", command);
      }
      continue;
    } else if ((strncmp(command, "ls", 2) == 0) |
               (strncmp(command, "dir", 3) == 0) |
               (strncmp(command, "get", 3) == 0) |
               (strncmp(command, "put", 3) == 0)) {
      char order[100];
      bzero(order, strlen(order));
      memcpy(order, command, strlen(command));

      while (1) {
        printf("Please input PORT or PASV: \n");

        /*printf("ftp> ");*/
        bzero(command, strlen(command));
        if (fgets(command, read_len, stdin) == NULL) {
          printf("Input Error!\n");
          break;
        }
        command[strlen(command) - 1] = '\0';
        printf("Input Command is [%s]\n", command);

        if (strncmp(command, "PORT", 4) == 0) {
          command_port(&control_sock, order);
        } else if (strncmp(command, "PASV", 4) == 0) {
          command_pasv(&control_sock, order);
        } else {
          printf("Input Error!\n");
          continue;
        }
        break;
      }
    } else {
      printf("Command Is Error! Please Try Again!\n");
    }
  }
  return 0;
}

void command_help() {

  printf("\n=---------------------欢迎使用FTP-------------------------|\n");
  printf("|                                                           |\n");
  printf("|                 ?:显示所有FTP服务器命令                   |\n");
  printf("|                                                           |\n");
  printf("|                 quit:离开FTP服务器                        |\n");
  printf("|                                                           |\n");
  printf("|                 ls|dir: 显示FTP服务器的文件列表           |\n");
  printf("|                                                           |\n");
  printf("|                 pwd: 显示服务器当前目录                   |\n");
  printf("|                                                           |\n");
  printf("|                 cd: 改变服务器当前目录                    |\n");
  printf("|                                                           |\n");
  printf("|                 get <file>:从FTP服务器下载文件            |\n");
  printf("|                                                           |\n");
  printf("|                 put <file>:上传文件到FTP服务器            |\n");
  printf("|                                                           |\n");
  printf("|-----------------------------------------------------------|\n");
}

void command_pasv(int *control_sock, char *order) {
  // 被动模式
  /*sprintf(command, "PASV\r\n");*/
  sprintf(command, "PASV");
  write(*control_sock, command, strlen(command));
  /* 客户端接收服务器的响应码和信息，正常为 ”227 Entering Passive Mode
   * (h1,h2,h3,h4,p1,p2).” */
  bzero(command, sizeof(command));
  read(*control_sock, command, read_len);
  printf("%s\n", command);
  if (strstr(command, "227") == NULL) {
    printf("PASV error\n");
    exit(1);
  }

  /* 客户端解析服务器的响应码和信息，获取服务器的端口号 */

  // 获取IP地址
  char *p = strstr(command, "(") + 1;
  char *q = strstr(command, ",");
  char ip[20];
  bzero(ip, sizeof(ip));
  memcpy(ip, p, q - p);
  printf("IP is: %s\n", ip);
  // 获取端口号
  p = q + 1;
  q = strstr(command, ")");
  char port[20];
  bzero(port, sizeof(port));
  memcpy(port, p, q - p);
  printf("Port is: %s\n", port);

  // 创建数据连接套接字
  int data_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (data_sock < 0) {
    printf("socket error\n");
    exit(1);
  }

  // 设置数据连接套接字的地址
  struct sockaddr_in data_addr;
  data_addr.sin_family = AF_INET;
  data_addr.sin_port = htons(atoi(port));
  data_addr.sin_addr.s_addr = inet_addr(ip);

  // 连接数据套接字
  if (connect(data_sock, (struct sockaddr *)&data_addr, sizeof(data_addr)) <
      0) {
    printf("connect error\n");
    exit(1);
  }

  if ((strncmp(order, "ls", 2) == 0) | (strncmp(order, "dir", 3) == 0)) {
    command_ls(control_sock, &data_sock, order);
  } else if (strncmp(order, "get", 3) == 0) {
    command_get(control_sock, &data_sock, order);
  } else if (strncmp(order, "put", 3) == 0) {
    command_put(control_sock, &data_sock, order);
  }
}

void command_port(int *control_socket, char *order) {
  // 用主动模式从 FTP 服务器下载文件
  int data_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data_socket < 0) {
    printf("socket error\n");
    exit(1);
  }
  int data_port;

  struct sockaddr_in data_addr;
  int data_len = sizeof(data_addr);
  bzero(&data_addr, sizeof(data_addr));
  data_addr.sin_family = AF_INET;
  data_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  data_addr.sin_port = 0;
  if (bind(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0) {
    printf("bind error\n");
    exit(1);
  }
  if (getsockname(data_socket, (struct sockaddr *)&data_addr, &data_len) < 0) {
    printf("getsockname error\n");
    exit(1);
  }
  data_port = ntohs(data_addr.sin_port);
  printf("data_port is: %d\n", data_port);

  bzero(command, sizeof(command));
  /*sprintf(command, "PORT %d,%d,%d,%d,%d,%d", ip[0], ip[1], ip[2], ip[3],*/
  /*port[0], port[1]);*/
  sprintf(command, "PORT %d", data_port);
  write(*control_socket, command, strlen(command));
  /* 客户端接收服务器的响应码和信息，正常为 ”200 Command okay.” */
  /*
  bzero(command, sizeof(command));
  read(*control_socket, command, read_len);
  printf("%s\n", command);
  if (strstr(command, "200") == NULL) {
    printf("PORT error\n");
    exit(1);
  }
  */

  if (listen(data_socket, 1) < 0) {
    printf("Data_socket listen error\n");
    exit(1);
  }

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  printf("do data server>");
  if ((client_socket =
           accept(data_socket, (struct sockaddr *)&data_addr, &data_len)) < 0) {
    printf("Data_socket Accept Error!\n");
  } else {
    printf("Accept Success!\n");
    /*printf("do data server>");*/

    if ((strncmp(order, "ls", 2) == 0) | (strncmp(order, "dir", 3) == 0)) {
      command_ls(control_socket, &client_socket, order);
    } else if (strncmp(order, "get", 3) == 0) {
      command_get(control_socket, &client_socket, order);
    } else if (strncmp(order, "put", 3) == 0) {
      command_put(control_socket, &client_socket, order);
    }
    close(data_socket);
  }
}

void command_ls(int *control_sock, int *data_sock, char *order) {
  bzero(command, sizeof(command));
  write(*control_sock, order, strlen(order));

  int size;
  recv(*data_sock, &size, sizeof(int), 0);
  char *f = malloc(size);
  recv(*data_sock, f, size, 0);
  int filehandle = creat("temp.txt", 0666);
  /*write(filehandle, f, size, 0);*/
  write(filehandle, f, size);
  close(filehandle);
  printf("The remote directory listing is as follows:\n");
  system("cat temp.txt");

  close(*data_sock);
}

void command_get(int *control_sock, int *data_sock, char *order) {
  write(*control_sock, order, strlen(order));
  bzero(command, sizeof(command));
  // if ((fd = open(order + 4, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
  char *filename = order + 4;

  int size;
  int filehandle;
  recv(*data_sock, &size, sizeof(int), 0);
  if (!size) {
    printf("No such file on the remote directory\n\n");
    close(*data_sock);
    return;
  }
  char *f = malloc(size);
  recv(*data_sock, f, size, 0);
  while (1) {
    filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
    if (filehandle == -1) {
      sprintf(filename + strlen(filename), "%d",
              1); // needed only if same directory is used for both server and
                  // client
    } else
      break;
  }
  write(filehandle, f, size);

  // 查看文件
  // strcpy(command, "cat ");
  // strcat(command, filename);
  // system(command);

  sleep(1); // 延时一秒，等待文件写入完成
  close(filehandle);
  close(*data_sock);
  printf("Get file success\n");
}

void command_put(int *control_sock, int *data_sock, char *order) {
  write(*control_sock, order, strlen(order));
  char *filename = order + 4;
  int filehandle = open(filename, O_RDONLY);
  if (filehandle == -1) {
    printf("No such file on the local directory\n\n");
    close(*data_sock);
    return;
  }

  struct stat obj;
  int status;
  stat(filename, &obj);
  int size = obj.st_size;
  send(*data_sock, &size, sizeof(int), 0);
  sendfile(*data_sock, filehandle, NULL, size);
  recv(*data_sock, &status, sizeof(int), 0);
  if (status)
    printf("File stored successfully\n");
  else
    printf("File failed to be stored to remote machine\n");

  sleep(1); // 延时一秒，等待文件写入完成
  close(*data_sock);
  printf("Put file success\n");
}
