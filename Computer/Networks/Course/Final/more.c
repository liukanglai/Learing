// Time:2022-06-16 08:13:23
#include <stdio.h>

int main(void) {}

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
  // 参数是要使用的数据连接端口，通常情况下对此不需要命令响应。如果使用此命令时，要发送32位的IP地址和16位的TCP端口号。上面的信息以8位为一组，逗号间隔十进制传输，如下例：

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

void login() {
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
}
