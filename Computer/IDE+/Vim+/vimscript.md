# vim 脚本文件

- 使用:去执行命令(单步调试) :source 执行整个脚本文件
- 后缀.vim
- :source *.vim (:so %)
- :echo variable(print it)

# grammar
> 无专门的boolean, 1 is true, 0 is false
1. variable
    1. set

    2. let 非内部变量的赋值
    - let g:hh='liu' g作用域
    - g全局（默认的） v（vim定义的全局作用域） l（局部，函数内部默认） b（当前缓冲区） w（当前窗口） t（当前标签页） s(使用:source'd执行的vim脚本文件中的局部文件作用域) a(函数的参数)
    - let @a='liu'
    - let &ignorecase = 0 (可用&访问set命令的变量)
2. 
3. function
4. loop


> 整数变量可用+-\*/, 字符串拼接使用.(如: 'h'.'ello'), 字符串内使用'(两个, '')
>> '内为字符串, "内可有非字面量字符, 但注释也使用, 所以有些命令后面不能用注释
# style

# plugin
