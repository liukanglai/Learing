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

// for directory
#include <dirent.h>

// for file
#include <fcntl.h>

/*for getting file size using stat()*/
#include <sys/stat.h>

/*for sendfile()*/
#include <sys/sendfile.h>

#define read_len 1024
char command[read_len];

void command_cd(int *, char *);
void command_pwd(int *, char *);
int command_pasv(int *, char *);
int command_port(int *, char *);
void command_ls(int *);
void command_get(int *);
void command_put(int *);

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

    // 验证用户名和密码
    bzero(command, strlen(command));
    read(client_socket, command, read_len);
    printf("received [%s]\n", command);
    if (strncmp(command, "USER", 4) == 0) {
      if (strncmp(command + 5, "liukanglai", 10) == 0) {
        write(client_socket, "331: User name okay, need password.\n",
              strlen("331: User name okay, need password.\n"));
      } else {
        write(client_socket, "The Username is wrong!.\n",
              strlen("The Username is wrong!.\n"));
        continue;
      }
    } else {
      write(client_socket, "530: Not logged in.\n",
            strlen("530: Not logged in.\n"));
      continue;
    }
    bzero(command, strlen(command));
    read(client_socket, command, read_len);
    printf("received [%s]\n", command);
    if (strncmp(command, "PASS", 4) == 0) {
      if (strncmp(command + 5, "123456", 6) == 0) {
        write(client_socket, "230: User logged in, proceed.\n",
              strlen("230: User logged in, proceed.\n"));
      } else {
        write(client_socket, "530: Not logged in.\n",
              strlen("530: Not logged in.\n"));
        continue;
      }
    } else {
      write(client_socket, "530: Not logged in.\n",
            strlen("530: Not logged in.\n"));
      continue;
    }

    while (1) {
      bzero(command, strlen(command));
      printf("server(%s:%d)> ", ip, client_port);

      if (read(client_socket, command, read_len) < 0) {
        printf("Read Error!\n");
        continue;
      }

      printf("received [%s]\n", command);

      if (strncmp(command, "quit", 4) == 0) {
        /* 客户端接收服务器的响应码，正常为 ”200 Closes connection.” */
        close(client_socket);
        break;
      } else if (strncmp(command, "cd", 2) == 0) {
        /* 客户端接收服务器的响应码和信息，正常为 ”250 Command okay.” */
        command_cd(&client_socket, command + 3);
        continue;
      } else if (strncmp(command, "pwd", 3) == 0) {
        command_pwd(&client_socket, command);
        continue;
      } else if (strncmp(command, "PORT", 4) == 0) {
        /*command_port(&client_socket, command);*/
        continue;
      } else if (strncmp(command, "PASV", 4) == 0) {
        command_pasv(&client_socket, command);
        continue;
      } else {
        printf("Command Error!\n");
      }
    }
    close(client_socket);
  }

  return 0;
}

void command_pwd(int *sockfd, char *buf) {
  bzero(buf, strlen(buf));
  getcwd(buf, 100);
  write(*sockfd, buf, strlen(buf));
}

void command_cd(int *sockfd, char *buf) {
  if (chdir(buf) < 0) {
    write(*sockfd, "Change Directory Error!\n",
          strlen("Change Directory Error!\n"));
  } else {
    bzero(buf, strlen(buf));
    getcwd(buf, 100);
    write(*sockfd, buf, strlen(buf));
  }
}

int command_pasv(int *control_socket, char *command) {
  int data_socket;
  if ((data_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Create Socket Error!\n");
    return -1;
  }
  int data_port;

  struct sockaddr_in data_addr;
  int data_len = sizeof(data_addr);
  bzero(&data_addr, data_len);
  data_addr.sin_family = AF_INET;
  data_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  data_addr.sin_port = 0; // 由系统分配端口
  if (bind(data_socket, (struct sockaddr *)&data_addr, data_len) < 0) {
    printf("Bind Error!\n");
    return -1;
  }
  if (getsockname(data_socket, (struct sockaddr *)&data_addr, &data_len) < 0) {
    printf("Getsockname Error!\n");
    return -1;
  }
  data_port = ntohs(data_addr.sin_port);
  printf("data_port: %d\n", data_port);
  /*write(*control_socket, "227 Entering Passive Mode (",*/
  /*strlen("227 Entering Passive Mode ("));*/

  char *ip;
  ip = inet_ntoa(data_addr.sin_addr);
  strcpy(command, "227 Entering Passive Mode (");
  strcat(command, ip);
  strcat(command, ",");
  printf("ip: %s\n", ip);

  /*sprintf(ip, "%d.%d.%d.%d", ip_addr[0], ip_addr[1], ip_addr[2],
   * ip_addr[3]);*/
  /*getlocalip(ip);*/
  /*write(*control_socket, ip, strlen(ip));*/
  /*write(*control_socket, ",", 1);*/
  /*strcpy(command, "227 Entering Passive Mode (0,0,0,0,");*/
  char port[10];
  bzero(port, strlen(port));
  sprintf(port, "%d", data_port);
  strcat(command, port);
  strcat(command, ")\n");
  write(*control_socket, command, strlen(command));
  /*write(*control_socket, port, strlen(port));*/
  /*write(*control_socket, ").\n", strlen(").\n"));*/

  if (listen(data_socket, 1) < 0) {
    printf("Listen Error!\n");
    return -1;
  } else {
    /*printf("Listening...\n");*/
  }

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  printf("server>");
  if ((client_socket =
           accept(data_socket, (struct sockaddr *)&data_addr, &data_len)) < 0) {
    printf("Accept Error!\n");
    return -1;
  } else {
    printf("Accept Success!\n");
    printf("server>");

    bzero(command, strlen(command));
    if (read(*control_socket, command, read_len) < 0) {
      printf("Read Command Error!\n");
    }
    printf("received [%s]\n", command);

    if (strncmp(command, "get", 3) == 0) {
      /*command_get(&data_socket);*/
      command_get(&data_socket);
    } else if (strncmp(command, "put", 3) == 0) {
      /*command_put(&data_socket);*/
      command_put(&client_socket);
    } else if ((strncmp(command, "ls", 2) == 0) |
               (strncmp(command, "dir", 3) == 0)) {
      /*command_ls(&data_socket);*/
      command_ls(&client_socket);
    }
  }
  close(data_socket);
  return 0;
}

/*
int command_port(int *sockfd, char *buf) {

  // 记录客户端的 ip 地址
  int client_socket;
  struct sockaddr_in client_addr;
  bzero(&client_addr, sizeof(client_addr));
  int client_len = sizeof(client_addr);

  bzero(command, read_len); // 字符串的前字节置为0，包括'\0'
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
             inet_ntop(AF_INET, &client_addr.sin_addr, command,
                       sizeof(command)), // 将网络字节序转换为点分十进制字符串
             htons(client_addr.sin_port));
      write(client_socket,
            "220: Service ready for new user.!\nWelcome to my FTP Server!\n",
            strlen("220: Service ready for new user.!\nWelcome to my FTP "
                   "Server!\n"));
      printf("server>");
    }

    if (read(client_socket, command, read_len) < 0) {
      //
由套接字接收数据时，套接字把接收的数据放在套接字缓冲区，再由用户程序把它们复制到用户缓冲区，然后由read函数读取
      // write函数同理
      printf("Read Command Error!\n");
      continue;
    }
    printf("received [ %s ]\n", command);

    if (strncmp(command, "get", 3) == 0) {
    } else if (strncmp(command, "put", 3) == 0) {
      continue;
    } else {
      printf("Command Error!\n");
    }

    if (strncmp(command, "get", 3) == 0) {

    } else if (strncmp(command, "put", 3) == 0) {
      bzero(command, strlen(command));
      read(client_socket, command, read_len);
      printf("received [%s]\n", command);
      continue;
    } else if ((strncmp(command, "ls", 2) == 0) |
               (strncmp(command, "dir", 3) == 0)) {
      printf("Command Error!\n");
    }
  }
}
*/

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
    bzero(command, strlen(command));
    if (sprintf(command, myitem->d_name, read_len) < 0) //把文件名写入缓冲区
    {
      printf("Sprintf Error!\n");
      exit(1);
    }

    if (write(*socket, command, strlen(command)) <
        0) //将commd缓冲区的内容发送会client
    {
      printf("Write Error!\n");
      exit(1);
    }
  }

  closedir(mydir); //关闭目录流
  close(*socket);  //关闭套接字
  printf("ls success!\n");

  return;
}
/*
 if(!strcmp(command, "ls"))
        {
          system("ls >temps.txt");
          i = 0;
          stat("temps.txt",&obj);
          size = obj.st_size;
          send(sock2, &size, sizeof(int),0);
          filehandle = open("temps.txt", O_RDONLY);
          sendfile(sock2,filehandle,NULL,size);
        }
  */

void command_get(int *socket) {
  char *filename = command + 4;
  int fd, nbytes;

  printf("get filename : [%s]\n", filename);
  if ((fd = open(filename, O_RDONLY)) < 0) //以只读的方式打开client要下载的文件
  {
    printf("Open file Error!\n");
    return;
  }

  bzero(command, strlen(command));
  while ((nbytes = read(fd, command, read_len)) == 0) { // 重要！！！
    if (write(*socket, command, strlen(command)) < 0) {
      printf("Write Error!\n");
      exit(1);
    }
    /*write(*socket, "File is empty!\n",*/
    /*strlen("File is empty!\n")); // PIPE wrong???*/
  }
  while ((nbytes = read(fd, command, read_len)) > 0) {
    if (write(*socket, command, nbytes) < 0) {
      printf("Write Error! At commd_get 3!\n");
      close(fd);
      exit(1);
    }
  }

  close(fd);
  close(*socket);
}
/*
  else if(!strcmp(command,"get"))
        {
          sscanf(buf, "%s%s", filename, filename);
          stat(filename, &obj);
          filehandle = open(filename, O_RDONLY);
          size = obj.st_size;
          if(filehandle == -1)
              size = 0;
          send(sock2, &size, sizeof(int), 0);
          if(size)
          sendfile(sock2, filehandle, NULL, size);

    */

void command_put(int *socket) {
  char *filename = command + 4;
  int fd, nbytes;

  printf("get filename : [ %s ]\n", filename);
  if ((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644)) <
      0) //以只写的方式打开文件，若文件存在则清空，若文件不存在则新建文件
  {
    printf("Open file Error!\n");
    return;
  }

  bzero(command, strlen(command));
  while ((nbytes = read(*socket, command, read_len)) == 0) { // ???
    if (write(fd, command, nbytes) < 0) {
      printf("Write Error! At commd_put 1!\n");
      close(fd);
      exit(1);
    }
  }
  while ((nbytes = read(*socket, command, read_len)) > 0) {
    if (write(fd, command, nbytes) < 0) {
      printf("Write Error! At commd_put 1!\n");
      close(fd);
      exit(1);
    }
  }
  close(fd);
  close(*socket);
}
/*
else if(!strcmp(command, "put"))
        {
          int c = 0, len;
          char *f;
          sscanf(buf+strlen(command), "%s", filename);
          recv(sock2, &size, sizeof(int), 0);
          i = 1;
          while(1)
            {
              filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
              if(filehandle == -1)
                {
                  sprintf(filename + strlen(filename), "%d", i);
                }
              else
                break;
            }
          f = malloc(size);
          recv(sock2, f, size, 0);
          c = write(filehandle, f, size);
          close(filehandle);
          send(sock2, &c, sizeof(int), 0);
        }
*/
