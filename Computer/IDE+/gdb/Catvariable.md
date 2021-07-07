# cat variable

- p name
- examine(简写为x)可以用来查看内存地址中的值。语法如下：
- auto display: display name

x/[n][f][u] addr
其中：

n 表示要显示的内存单元数，默认值为1
f 表示要打印的格式，前面已经提到了格式控制字符
u 要打印的单元长度
addr 内存地址
单元类型常见有如下：

b 字节
h 半字，即双字节
w 字，即四字节
g 八字节

