// Time:2022-06-15 16:25:20
#include <stdio.h>

// inet_ntop
#include <arpa/inet.h>
#include <netinet/in.h>
// bzero
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define read_len 1024
char read_buf[read_len];

int main(void) {
  int server_control_socket;
  struct sockaddr_in server_control_addr;
  int server_control_len = sizeof(server_control_addr);

  // 初始化 control_socket
  if ((server_control_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return 1;
  }
  bzero(&server_control_addr,
        sizeof(server_control_addr)); // 字符串的前字节置为0，包括'\0'
  server_control_addr.sin_family = AF_INET;
  server_control_addr.sin_addr.s_addr =
      htonl(INADDR_ANY);                      // 本地ip地址 (0.0.0.0)
  server_control_addr.sin_port = htons(2001); // 转换成网络字节

  // 将ip地址与套接字绑定
  if ((bind(server_control_socket, (struct sockaddr *)&server_control_addr,
            server_control_len)) < 0) {
    printf("Bind Error!\n");
    return -1;
  }

  // 服务器端监听
  if (listen(server_control_socket, 5) < 0) {
    printf("Linsten Error!\n");
    return -1;
  } else {
    printf("Listening...\n");
  }

  // 记录客户端的ip地址
  int client_control_socket;
  struct sockaddr_in client_control_addr;
  bzero(&client_control_addr, sizeof(client_control_addr));
  int client_control_len = sizeof(client_control_addr);

  char read_buf[read_len];
  bzero(read_buf, read_len); // 字符串的前字节置为0，包括'\0'

  while (1) {
    bzero(read_buf, read_len); // 字符串的前字节置为0，包括'\0'
    printf("server>");
    // 服务器端接受来自客户端的连接，返回一个套接字，此套接字为新建的一个
    // 并将客户端的地址等信息存入client_control_addr中
    // 原来的套接字仍处于监听中
    if ((client_control_socket = accept(server_control_socket,
                                        (struct sockaddr *)&client_control_addr,
                                        &client_control_len)) < 0) {
      printf("Accept Error!\n");
      exit(1);
    } else {
      printf("Accept Success!\n");
      printf("Connect with % sPORT % d\n",
             inet_ntop(AF_INET, &client_control_addr.sin_addr, read_buf,
                       sizeof(read_buf)), // 将网络字节序转换为点分十进制字符串
             htons(client_control_addr.sin_port));
      write(client_control_socket, "220: Service ready for new user.!\n",
            strlen("220: Service ready for new user.!\n"));
      write(client_control_socket, "Welcome to my FTP Server!\n",
            strlen("Welcome to my FTP Server!\n"));
    }
    bzero(read_buf, read_len); // 字符串的前字节置为0，包括'\0'

    // 由套接字接收数据时，套接字把接收的数据放在套接字缓冲区，再由用户程序把它们复制到用户缓冲区，然后由read函数读取
    // write函数同理
    if (read(client_control_socket, read_buf, read_len) <
        0) // read函数从cli_sockfd中读取N个字节数据放入read_buf中
    {
      printf("Read Error!\n");
      exit(1);
    }

    printf("received [ %s ]\n", read_buf);

    if ((strncmp(read_buf, "ls", 2) == 0) |
        (strncmp(read_buf, "dir", 3) == 0)) {
      command_ls(client_control_socket);
    } else if (strncmp(read_buf, "pwd", 3) == 0) {
      command_pwd(client_control_socket, read_buf + 4);
    } else if (strncmp(read_buf, "cd", 2) == 0) {
      command_cd(client_control_socket, read_buf + 4);
    } else if (strncmp(read_buf, "get", 3) == 0) {
      command_get(client_control_socket, read_buf + 4);
    } else if (strncmp(read_buf, "put", 3) == 0) {
      command_put(client_control_socket, read_buf + 4);
    } else {
      printf("Command Error!\n");
    }
  }

  return 0;
}

/*
void commd_ls(int sockfd)
{
    DIR * mydir =NULL;
    struct dirent *myitem = NULL;
    char commd[N] ;
    bzero(commd, N);
    //opendir为用来打开参数name 指定的目录, 并返回DIR*形态的目录流
    //mydir中存有相关目录的信息
    if((mydir=opendir(".")) == NULL)
    {
        printf("OpenDir Error!\n");
        exit(1);
    }

    while((myitem = readdir(mydir)) !=
NULL)//用来读取目录,返回是dirent结构体指针
    {
        if(sprintf(commd, myitem->d_name, N) <
0)//把文件名写入commd指向的缓冲区
        {
            printf("Sprintf Error!\n");
            exit(1);
        }

        if(write(sockfd, commd, N) < 0 )//将commd缓冲区的内容发送会client
        {
            printf("Write Error!\n");
            exit(1);
        }
    }

    closedir(mydir);//关闭目录流
    close(sockfd);

    return ;
}
*/
