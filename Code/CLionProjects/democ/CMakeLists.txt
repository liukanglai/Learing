cmake_minimum_required(VERSION 3.20)
project(democ C)

set(CMAKE_C_STANDARD 99)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
#aux_source_directory(. DIR_SRCS)

# 指定生成目标
#add_executable(democ ${DIR_SRCS})
add_executable(democ OS/final-src-osc10e/ch3/my_shell.c)

target_link_libraries(democ  m)