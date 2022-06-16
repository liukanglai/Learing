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

// for opendir
#include <dirent.h>

#define read_len 1024
// send read 共用
char read_buf[read_len];

void command_ls(int *);
void command_pwd(int *, char *);
void command_get(int *, char *);
void command_put(int *, char *);
int main(void) {
  int control_socket;
  struct sockaddr_in control_addr;
  int control_len = sizeof(control_addr);

  // 初始化 control_socket
  if ((control_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return 1;
  }
  bzero(&control_addr,
        sizeof(control_addr)); // 字符串的前字节置为0，包括'\0'
  control_addr.sin_family = AF_INET;
  control_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 本地ip地址 (0.0.0.0)
  control_addr.sin_port = htons(2001);              // 转换成网络字节

  // 将ip地址与套接字绑定
  if ((bind(control_socket, (struct sockaddr *)&control_addr, control_len)) <
      0) {
    printf("Bind Error!\n");
    return -1;
  }

  // 服务器端监听
  if (listen(control_socket, 1) < 0) {
    printf("Linsten Error!\n");
    return -1;
  }

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  while (1) {
    /*bzero(read_buf, strlen(read_buf)); // 字符串的前字节置为0，包括'\0'*/
    printf("Waiting for connection...\n");
    // 服务器端接受来自客户端的连接，返回一个套接字，此套接字为新建的一个
    // 并将客户端的地址等信息存入client_addr中
    if ((client_socket = accept(control_socket, (struct sockaddr *)&client_addr,
                                &client_len)) < 0) {
      printf("Accept Error!\n");
      exit(1);
    }
    char ip[20];
    memcpy(ip, inet_ntoa(client_addr.sin_addr), sizeof(ip));
    int client_port = htons(client_addr.sin_port);

    printf("Accept Success!\n");
    printf("Connect with %s PORT: %d\n",
           // inet_ntop(AF_INET, &client_addr.sin_addr, read_buf,
           // sizeof(read_buf)), // 将网络字节序转换为点分十进制字符串
           ip, client_port);
    write(client_socket,
          "220: Service ready for new user.!\nWelcome to my FTP Server!\n",
          strlen("220: Service ready for new user.!\nWelcome to my FTP "
                 "Server!\n"));

    while (1) {
      printf("server(%s:%d)> ", ip, client_port);

      bzero(read_buf, strlen(read_buf));
      if (read(client_socket, read_buf, read_len) < 0) {
        // 由套接字接收数据时，套接字把接收的数据放在套接字缓冲区，再由用户程序把它们复制到用户缓冲区，然后由read函数读取
        printf("Read Command Error!\n");
        continue;
      }
      printf("received [%s]\n", read_buf);

      if (strncmp(read_buf, "quit", 4) == 0) {
        close(client_socket);
        break;
      } else if ((strncmp(read_buf, "ls", 2) == 0) |
                 (strncmp(read_buf, "dir", 3) == 0)) {
        command_ls(&client_socket);
        printf("hh\n");
        continue;
      } else if (strncmp(read_buf, "pwd", 3) == 0) {
        /*command_pwd(client_socket, read_buf + 4);*/
        continue;
      } else if (strncmp(read_buf, "cd", 2) == 0) {
        /*command_cd(client_socket, read_buf + 4);*/
        continue;
      } else if ((strncmp(read_buf, "get", 3) == 0) |
                 (strncmp(read_buf, "put", 3) == 0)) {
        /*command_get(client_socket, read_buf + 4);*/
        write(client_socket, "Please input POST or PASV first!\n",
              strlen("Please input POST or PASV first!\n"));
        continue;
      } else if (strncmp(read_buf, "POST", 4) == 0) {
        /*command_put(client_socket, read_buf + 4);*/
        continue;
      } else if (strncmp(read_buf, "PASV", 4) == 0) {
        /*command_put(client_socket, read_buf + 4);*/
        continue;
      } else {
        printf("Command Error!\n");
      }
    }
  }

  return 0;
}

void command_ls(int *socket) {
  DIR *mydir = NULL;
  struct dirent *myitem = NULL;
  // opendir为用来打开参数name 指定的目录, 并返回DIR*形态的目录流
  // mydir中存有相关目录的信息
  if ((mydir = opendir(".")) == NULL) {
    printf("OpenDir Error!\n");
    exit(1);
  }

  while ((myitem = readdir(mydir)) !=
         NULL) //用来读取目录,返回是dirent结构体指针
  {
    if (sprintf(read_buf, myitem->d_name, read_len) < 0) //把文件名写入缓冲区
    {
      printf("Sprintf Error!\n");
      exit(1);
    }

    if (write(*socket, read_buf, strlen(read_buf)) <
        0) //将commd缓冲区的内容发送会client
    {
      printf("Write Error!\n");
      exit(1);
    }
  }

  closedir(mydir); //关闭目录流
  printf("ls success!\n");

  return;
}

/*
void command_pwd(int sockfd, char *buf) {
  char pwd[];
  bzero(pwd, N);
  getcwd(pwd, N);
  write(sockfd, pwd, strlen(pwd));
  return;
}
*/

void command_cd(int sockfd, char *buf) {
  if (chdir(buf) < 0) {
    write(sockfd, "Change Directory Error!\n",
          strlen("Change Directory Error!\n"));
  } else {
    write(sockfd, "Change Directory Success!\n",
          strlen("Change Directory Success!\n"));
  }
  return;
}

int command_POST(int sockfd, char *buf) {
  int control_socket;
  struct sockaddr_in control_addr;
  int control_len = sizeof(control_addr);

  // 初始化 control_socket
  if ((control_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return 1;
  }
  bzero(&control_addr,
        sizeof(control_addr)); // 字符串的前字节置为0，包括'\0'
  control_addr.sin_family = AF_INET;
  control_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 本地ip地址 (0.0.0.0)
  control_addr.sin_port = htons(2001);              // 转换成网络字节

  // 将ip地址与套接字绑定
  if ((bind(control_socket, (struct sockaddr *)&control_addr, control_len)) <
      0) {
    printf("Bind Error!\n");
    return -1;
  }

  // 服务器端监听
  if (listen(control_socket, 5) < 0) {
    printf("Linsten Error!\n");
    return -1;
  } else {
    printf("Listening...\n");
  }

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  bzero(read_buf, read_len); // 字符串的前字节置为0，包括'\0'
  while (1) {
    printf("server>");
    // 服务器端接受来自客户端的连接，返回一个套接字，此套接字为新建的一个
    // 并将客户端的地址等信息存入client_addr中
    // 原来的套接字仍处于监听中
    if ((client_socket = accept(control_socket, (struct sockaddr *)&client_addr,
                                &client_len)) < 0) {
      printf("Accept Error!\n");
      exit(1);
    } else {
      printf("Accept Success!\n");
      printf("Connect with %s PORT: %d\n",
             inet_ntop(AF_INET, &client_addr.sin_addr, read_buf,
                       sizeof(read_buf)), // 将网络字节序转换为点分十进制字符串
             htons(client_addr.sin_port));
      write(client_socket,
            "220: Service ready for new user.!\nWelcome to my FTP Server!\n",
            strlen("220: Service ready for new user.!\nWelcome to my FTP "
                   "Server!\n"));
      printf("server>");
    }

    if (read(client_socket, read_buf, read_len) < 0) {
      // 由套接字接收数据时，套接字把接收的数据放在套接字缓冲区，再由用户程序把它们复制到用户缓冲区，然后由read函数读取
      // write函数同理
      printf("Read Command Error!\n");
      continue;
    }
    printf("received [ %s ]\n", read_buf);

    if (strncmp(read_buf, "quit", 4) == 0) {
      close(client_socket);
      exit(0);
    } else if ((strncmp(read_buf, "ls", 2) == 0) |
               (strncmp(read_buf, "dir", 3) == 0)) {
      /*command_ls(client_socket);*/
      continue;
    } else if (strncmp(read_buf, "pwd", 3) == 0) {
      /*command_pwd(client_socket, read_buf + 4);*/
      continue;
    } else if (strncmp(read_buf, "cd", 2) == 0) {
      /*command_cd(client_socket, read_buf + 4);*/
      continue;
    } else if ((strncmp(read_buf, "get", 3) == 0) |
               (strncmp(read_buf, "put", 3) == 0)) {
      continue;
    } else if ((strncmp(read_buf, "POST", 4) == 0) |
               (strncmp(read_buf, "PASV", 4) == 0)) {
      write(client_socket, "Already in PASV mode!\n",
            strlen("Already in PASV mode!\n"));
      continue;
    } else {
      printf("Command Error!\n");
    }
  }
}

int command_PASV(int sockfd, char *buf) {
  int control_socket;
  struct sockaddr_in control_addr;
  int control_len = sizeof(control_addr);

  // 初始化 control_socket
  if ((control_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return 1;
  }
  bzero(&control_addr,
        sizeof(control_addr)); // 字符串的前字节置为0，包括'\0'
  control_addr.sin_family = AF_INET;
  control_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 本地ip地址 (0.0.0.0)
  control_addr.sin_port = htons(2001);              // 转换成网络字节

  // 将ip地址与套接字绑定
  if ((bind(control_socket, (struct sockaddr *)&control_addr, control_len)) <
      0) {
    printf("Bind Error!\n");
    return -1;
  }

  // 服务器端监听
  if (listen(control_socket, 5) < 0) {
    printf("Linsten Error!\n");
    return -1;
  } else {
    printf("Listening...\n");
  }

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  bzero(read_buf, read_len); // 字符串的前字节置为0，包括'\0'
  while (1) {
    printf("server>");
    // 服务器端接受来自客户端的连接，返回一个套接字，此套接字为新建的一个
    // 并将客户端的地址等信息存入client_addr中
    // 原来的套接字仍处于监听中
    if ((client_socket = accept(control_socket, (struct sockaddr *)&client_addr,
                                &client_len)) < 0) {
      printf("Accept Error!\n");
      exit(1);
    } else {
      printf("Accept Success!\n");
      printf("Connect with %s PORT: %d\n",
             inet_ntop(AF_INET, &client_addr.sin_addr, read_buf,
                       sizeof(read_buf)), // 将网络字节序转换为点分十进制字符串
             htons(client_addr.sin_port));
      write(client_socket,
            "220: Service ready for new user.!\nWelcome to my FTP Server!\n",
            strlen("220: Service ready for new user.!\nWelcome to my FTP "
                   "Server!\n"));
      printf("server>");
    }

    if (read(client_socket, read_buf, read_len) < 0) {
      // 由套接字接收数据时，套接字把接收的数据放在套接字缓冲区，再由用户程序把它们复制到用户缓冲区，然后由read函数读取
      // write函数同理
      printf("Read Command Error!\n");
      continue;
    }
    printf("received [ %s ]\n", read_buf);

    if (strncmp(read_buf, "quit", 4) == 0) {
      close(client_socket);
      exit(0);
    } else if ((strncmp(read_buf, "ls", 2) == 0) |
               (strncmp(read_buf, "dir", 3) == 0)) {
      /*command_ls(client_socket);*/
      continue;
    } else if (strncmp(read_buf, "pwd", 3) == 0) {
      /*command_pwd(client_socket, read_buf + 4);*/
      continue;
    } else if (strncmp(read_buf, "cd", 2) == 0) {
      /*command_cd(client_socket, read_buf + 4);*/
      continue;
    } else if ((strncmp(read_buf, "get", 3) == 0) |
               (strncmp(read_buf, "put", 3) == 0)) {
      continue;
    } else if ((strncmp(read_buf, "POST", 4) == 0) |
               (strncmp(read_buf, "PASV", 4) == 0)) {
      write(client_socket, "Already in PASV mode!\n",
            strlen("Already in PASV mode!\n"));
      continue;
    } else {
      printf("Command Error!\n");
    }
  }
}
