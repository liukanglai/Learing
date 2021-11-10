# gcc

- gcc -o filename file.c -lm(libm.a/.so库)  or -L /usr/lib/..

- /usr/lib

## library
- .a: 静态库
- .so: 共享库



# make

- for multisoure file: Makefile

# Makefile

- too complex: cmake

# cmake

- generate a Makefile to make
- cross-plantform!!!

## CMakeLists.txt(学名：组态档)

- write it yourself
- can generate Makefile or projects/workspaces in Windows Visual C++ 

1. write CmakeLists.txt
2. run: cmake PATH or ccmake PATH to Makefile (PATH is the fold of CMakeLists.txt)
3. make to compile

# nmake

- use in Microsoft Visual Studio，need to install VS，just like make in linux.

