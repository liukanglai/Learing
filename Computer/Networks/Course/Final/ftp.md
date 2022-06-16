# 程序架构

- ./FTPClient.c 127.0.0.1
-
- ./FTPServer.c

# 实现方法

```
/* Description of data base entry for a single host.  */
struct hostent
{
  char *h_name;			/* Official name of host.  主机规范名*/
  char **h_aliases;		/* Alias list.  主机别名*/
  int h_addrtype;		/* Host address type.  ipv4 or ipv6*/
  int h_length;			/* Length of address.  ip 长度*/
  char **h_addr_list;		/* List of addresses from name server.  ip 地址*/
#ifdef __USE_MISC
# define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
#endif
};


```

# 实验测试结果

# 遇到的问题和解决办法、

- while(read) 不会停

# 思考题解答

1. 在 FTP 中,为什么要建立两个 TCP 连接来分别传送命令和数据?

- 命令和数据传输的分离使 FTP 更加高效。

- 控制连接使用非常简单的通讯规则。我们一次只需要传递一条命令行或一条响应行即可。

- 另一方的数据连接由于要传输的数据类型繁多，因此需要更多复杂的规则。

2. 主动方式和被动方式的主要区别是什么?为何要设计这两种方式?

FTP 客户端处于 NAT 或防火墙之后的主动模式:

- 客户端向服务器建立数据通道时，发送 POST 命令，并含有其开放的端口，但服务器 TCP 20 端口将会主动连接防火墙或 NAT 的端口，而不是客户端的端口，数据通道建立失败。

FTP 服务器处于 NAT 或防火墙之后的被动模式:

- 服务器返回一个高端端口给客户端，客户端打开一个随机端口与服务器连接，但此时客户端将会与防火墙或者 NAT 的此高端端口连接，连接失败。

3. 当使用 FTP 下载大量小文件的时候,速度会很慢,这是什么缘故?可以怎样改进?

- 新开 TCP 连接，
- 使用一个 TCP 连接
