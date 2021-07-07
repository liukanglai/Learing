- file bin   加载名为 bin 的二进制文件
- CTRL-C     中断程序
- run/r      运行
- next/n     执行当前行，停在下一行 （step over）
- step/s     执行当前行，进入下一层函数 （step in）
- finish     执行直至离开当前函数
- where      显示栈
- continue/c 继续执行
- break/b N  在第 N 行加断点
- break/b f  在函数 f 处加断点
- delete     删除所有断点
# create file 

- gcc -g -o hello hello.c

# run

- gdb hello

# cat file

- list
- l

# breakpoints

- info breakpoints
- b line or b test.c line
- b function
- disable
- enable
- auto display: display name

# Start

- b
- run(now you can input)
- n 2(do 2)
- s 2(enter function)
- u 行号
