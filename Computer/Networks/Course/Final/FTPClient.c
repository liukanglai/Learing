// Time:2022-06-15 17:02:09
#include <stdio.h>

// for memset
#include <string.h>
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

#define read_len 1024
char read_buf[read_len];
char send_buf[read_len];
char command[read_len];

void command_help();
void command_ls(struct sockaddr_in, char *);
void command_pwd(struct sockaddr_in, char *);
void command_get(struct sockaddr_in, char *);
void command_put(struct sockaddr_in, char *);

// 客户端与服务器的命令通道和数据通道需要分离
int main(int argc, char *argv[]) {
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
  printf("Please input the port you intend to connect: ");
  scanf("%s", server_ip);
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  printf("Please input the port you intend to connect: ");
  scanf("%d", &server_port);
  // The htons() function converts the unsigned short integer hostshort from
  // host byte order to network byte order. 转换成网络字节
  server_addr.sin_port = htons(server_port);

  /* 连接到服务器端 */
  if ((connect(control_sock, (struct sockaddr *)&server_addr,
               sizeof(server_addr))) < 0) {
    printf("Connect Error!\n");
    return -1;
  }
  /* 客户端接收服务器端的一些欢迎信息 */
  read(control_sock, read_buf, read_len);
  puts(read_buf);

  // 登录
  /* 命令 ”USER username\r\n” */
  char username = ' ';
  printf("Please input username: ");
  scanf("%d", &username);
  sprintf(send_buf, "USER %s\r\n", username);
  /*客户端发送用户名到服务器端 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 接收服务器的响应码和信息，正常 "331 User name okay, need password." */
  read(control_sock, read_buf, read_len);
  /* 命令 ”PASS password\r\n” */
  char password = ' ';
  printf("Please input password: ");
  scanf("%d", &password);
  sprintf(send_buf, "PASS %s\r\n", password);
  /* 客户端发送密码到服务器端 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”230 User logged in, proceed.” */
  read(control_sock, read_buf, read_len);

  while (1) {
    printf("ftp>");
    bzero(command, read_len);
    // fgets函数从stdin流中读取 read_len-1 个字符放入command中
    if (fgets(command, read_len, stdin) == NULL) {
      printf("Fgets Error!\n");
      return -1;
    }

    command[strlen(command) - 1] =
        '\0'; // fgets函数读取的最后一个字符为换行符，此处将其替换为'\0'

    printf("Input Command Is [ %s ]\n", command);

    if (strncmp(command, "?", 1) == 0) //比较两个字符串前4个字节，若相等则返回0
    {
      command_help();
    } else if (strncmp(command, "quit", 4) == 0) {
      printf("Bye!\n");
      exit(0); //结束进程
    } else if ((strncmp(command, "ls", 2) == 0) |
               (strncmp(read_buf, "dir", 3) == 0)) {
      //将commd指向的内容写入到sockfd所指的文件中，此处即指套接字
      if (write(sockfd, command, N) < 0) {
        printf("Write Error!\n");
        exit(1);
      }
      while (read(sockfd, command, read_len) >
             0) //从sockfd中读取N字节内容放入commd中，
      {         //返回值为读取的字节数
        printf(" %s ", command);
      }
      printf("\n");
    } else if (strncmp(command, "pwd", 3) == 0) {
      command_pwd(addr, command);
    } else if (strncmp(command, "cd", 3) == 0) {
      command_cd(addr, command);
    } else if (strncmp(command, "get", 3) == 0) {
      command_get(addr, command);
    } else if (strncmp(command, "put", 3) == 0) {
      command_put(addr, command);
    } else {
      printf("Command Is Error!Please Try Again!\n");
    }
  }

  return 0;
}

void commd_help() {

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

  return;
}

void port() {
  // 用主动模式从 FTP 服务器下载文件
  SOCKET data_sock;
  data_sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = htons(INADDR_ANY);
  server_port = p1 * 256 + p2;
  length = sizeof(name);
  name.sin_port = htons(server_port);
  bind(server_sock, (struct sockaddr *)&name, length);
  struct sockaddr_in client_name;
  length = sizeof(client_name);
  /* 客户端开始监听端口p1*256+p2 */
  listen(server_sock, 64);
  /* 命令 ”PORT \r\n” */
  sprintf(send_buf, "PORT 1287,0,0,1,%d,%d\r\n", p1, p2);
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”200 Port command successful” */
  read(control_sock, read_buf, read_len);
  sprintf(send_buf, "RETR filename.txt\r\n");
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”150 Opening data channel for file
   * transfer.” */
  read(control_sock, read_buf, read_len);
  /* ftp客户端接受服务器端的连接请求 */
  data_sock = accept(server_sock, (struct sockaddr *)&client_name, &length);
  ...... file_handle = open(disk_name, ROFLAGS, RWXALL);
  for (;;) {
    ...... read(data_sock, read_buf, read_len);
    write(file_handle, read_buf, read_len);
    ......
  }
  close(file_handle);
}

void pasv() {
  // 被动模式
  /* 命令 ”PASV\r\n” */
  sprintf(send_buf, "PASV\r\n");
  /* 客户端发送 PASV 命令到服务器端 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”227 Entering Passive Mode
   * (h1,h2,h3,h4,p1,p2).” */
  read(control_sock, read_buf, read_len);

  /* 客户端解析服务器的响应码和信息，获取服务器的端口号 */
  char *p = strstr(read_buf, "(h1,h2,h3,h4,p1,p2)");

  /* 连接服务器新开的数据端口 */
  connect(data_sock, (struct sockaddr *)&server, sizeof(server));

  /* 命令 ”CWD dirname\r\n” */
  sprintf(send_buf, "CWD %s\r\n", dirname);
  /* 客户端发送命令改变工作目录 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”250 Command okay.” */
  read(control_sock, read_buf, read_len);

  /* 命令 ”RETR filename\r\n” */
  sprintf(send_buf, "RETR %s\r\n", filename);
  /* 客户端发送命令从服务器端下载文件 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码和信息，正常为 ”150 Opening data connection.” */
  read(control_sock, read_buf, read_len);

  /* 客户端关闭数据连接 */
  close(data_sock);
  /* 客户端接收服务器的响应码和信息，正常为 ”226 Transfer complete.” */
  read(control_sock, read_buf, read_len);
  /* 命令 ”QUIT\r\n” */
  sprintf(send_buf, "QUIT\r\n");
  /* 客户端将断开与服务器端的连接 */
  write(control_sock, send_buf, strlen(send_buf));
  /* 客户端接收服务器的响应码，正常为 ”200 Closes connection.” */
  read(control_sock, read_buf, read_len);
  /* 客户端关闭控制连接 */
  close(control_sock);
}
