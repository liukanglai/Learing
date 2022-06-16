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
/*char send_buf[read_len];*/
char command[read_len];

void command_help();
void command_pwd(struct sockaddr_in, char *);
void command_get(struct sockaddr_in, char *);
void command_put(struct sockaddr_in, char *);

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
  // 如果读取的字符串中有换行符，则会被截断
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
  /* 客户端接收服务器端的一些欢迎信息 */
  while (read(control_sock, read_buf, read_len) > 0) {
    printf("%s", read_buf);
  }

  // stop here?
  while (1) {
    printf("ftp> ");
    /*bzero(command, strlen(command));*/
    /*bzero(read_buf, strlen(read_buf));*/
    // bzero(command, read_len);
    // fgets函数从stdin流中读取 read_len-1 个字符放入command中
    /*if (fgets(command, read_len, stdin) == NULL) {*/
    /*printf("Input Error!\n");*/
    /*break;*/
    /*}*/
    scanf("%s", command);
    getchar();

    /*command[strlen(command) - 1] =*/
    /*'\0'; // fgets函数读取的最后一个字符为换行符，此处将其替换为'\0'*/

    printf("Input Command is [%s]\n", command);

    if (strncmp(command, "?", 1) == 0) //比较两个字符串前4个字节，若相等则返回0
    {
      command_help();
      continue;
    } else if (strncmp(command, "quit", 4) == 0) {
      printf("Bye!\n");
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        continue;
      }
      close(control_sock);
      exit(0); //结束进程
    } else if ((strncmp(command, "ls", 2) == 0) |
               (strncmp(command, "dir", 3) == 0)) {
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        continue;
      }
      while (read(control_sock, read_buf, read_len) >
             0) { //返回值为读取的字节数
        printf(" %s ", read_buf);
      }
      printf("\n");
      continue;
    } else if (strncmp(command, "pwd", 3) == 0) {
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        continue;
      } else {
        printf("The path of the remote directory is: %s\n", read_buf);
      }
      continue;
    } else if (strncmp(command, "cd", 2) == 0) {
      if (write(control_sock, command, strlen(command)) < 0) {
        perror("write");
        printf("Remote directory failed to change!\n");
        continue;
      } else {
        printf("Remote directory successfully changed\n");
        printf("Now the path of the remote directory is: %s\n", read_buf);
      }
      continue;
    } else if (strncmp(command, "get", 3) == 0) {
      /*command_get(addr, command);*/
      continue;
    } else if (strncmp(command, "put", 3) == 0) {
      /*command_put(addr, command);*/
      continue;
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

/*
**实现文件的下载
void commd_get(struct sockaddr_in addr, char *commd)
{
    int fd;
    int sockfd;
    char buffer[N];
    int nbytes;
    //创建套接字，并进行错误检测
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket Error!\n");
        exit(1);
    }
    //connect函数用于实现客户端与服务端的连接,此处还进行了错误检测
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Connect Error!\n");
        exit(1);
    }
    //通过write函数向服务端发送数据
    if(write(sockfd, commd, N) < 0)
    {
        printf("Write Error!At commd_get 1\n");
        exit(1);
    }
    //利用read函数来接受服务器发来的数据
    if(read(sockfd, buffer, N) < 0)
    {
        printf("Read Error!At commd_get 1\n");
        exit(1);
    }
    //用于检测服务器端文件是否打开成功
    if(buffer[0] =='N')
    {
        close(sockfd);
        printf("Can't Open The File!\n");
        return ;
    }
    //open函数创建一个文件，文件地址为(commd+4)，该地址从命令行输入获取
    if((fd=open(commd+4, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0)
    {
        printf("Open Error!\n");
        exit(1);
    }
    //read函数从套接字中获取N字节数据放入buffer中，返回值为读取的字节数
    while((nbytes=read(sockfd, buffer, N)) > 0)
    {
        //write函数将buffer中的内容读取出来写入fd所指向的文件，返回值为实际写入的字节数
        if(write(fd, buffer, nbytes) < 0)
        {
            printf("Write Error!At commd_get 2");
        }
    }

    close(fd);
    close(sockfd);

    return ;

}
*/

/*
**实现文件的上传
void commd_put(struct sockaddr_in addr, char *commd)
{
    int fd;
    int sockfd;
    char buffer[N];
    int nbytes;
    //创建套接字
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket Error!\n");
        exit(1);
    }
    //客户端与服务端连接
    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Connect Error!\n");
        exit(1);
    }
    //从commd中读取N字节数据，写入套接字中
    if(write(sockfd, commd, N)<0)
    {
        printf("Wrtie Error!At commd_put 1\n");
        exit(1);
    }
    //open函数从(commd+4)中，读取文件路径，以只读的方式打开
    if((fd=open(commd+4, O_RDONLY)) < 0)
    {
        printf("Open Error!\n");
        exit(1);
    }
    //从fd指向的文件中读取N个字节数据
    while((nbytes=read(fd, buffer, N)) > 0)
    {
        //从buffer中读取nbytes字节数据，写入套接字中
        if(write(sockfd, buffer, nbytes) < 0)
        {
            printf("Write Error!At commd_put 2");
        }
    }

    close(fd);
    close(sockfd);

    return ;
}
*/
