cmake_minimum_required(VERSION 3.19)
project(democ C)

set(CMAKE_C_STANDARD 99)

add_executable(democ mosaic2.c)
target_link_libraries(democ  m)