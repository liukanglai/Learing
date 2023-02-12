# Makefile

## library

- .a: 静态库
- .so: 共享库

## make

- for multisoure file: Makefile

- thing_right := $(wildcard \*.o) \* 通配符一定用wildcard包裹

- too complex: cmake

- make clean:

```makefile
.PHONY: clean # 防止有一个文件为clean
clean:
  rm -f *.o $(TARGET)
```
