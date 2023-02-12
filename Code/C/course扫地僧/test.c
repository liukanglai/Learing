// Copyright

#include <stdio.h>
#include <stdlib.h>

// 排序
int main(int argc, char *argv[]) {
  int a[] = {33, 654, 4, 455, 6, 33, 4};
  int num = sizeof(a) / sizeof(a[0]);
  int tmp = 0;
  for (int i = 0; i < 7; i++) {
    for (int j = i + 1; j < 7; j++) {
      if (a[i] < a[j]) {
        tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
      }
    }
  }
  printf("hello\n");
  return 0;
}

void printArray(int *a, int nums) {} // int a[7]; 这个a也是一个指针！同*a

void sortArray(int *a, int nums) {}

// 变量 vs 常量
// 变量用来找内存空间(&a: 地址)，它自己存在代码区 a=10 同 *((int*)1000000) =10;
//
// int a = 10; a is 10, *a is wrong? &a is address.
// int *a; a is address, *a is 10, &a is address of pointer?
// int a[10]; a is &a[0], *a is 10, &a is ?.
//
// a: 首地址，&a: 同a不变，但它是整个数组的地址!, a+1:, &a+1
// 数组类型，数组指针？
// 数组a不可变,为了释放内存
//
// typedef: 数据结构（cpp可不用），对数据类型起别名，那对内存空间取别名？
// 函数也是数据类型？可typedef
//
// void *
//
// 内存四区，代码区：二进制文件，全局区(static)：OS释放，全局和静态变量，字符串常量在这，堆：自动分配释放，函数参数值，局部变量，栈：动态内存申请，程序员释放，程序结束OS也释放
//
// 指针
// 多级指针做形参，只是4个字节但内存，是个地址，不管它是几维
// 指针但数据类型是它指向数据的类型，步长也是。
// 野指针，已释放，但还用。方法：
// char *p1 = NULL; p1=(char*)malloc(100); if(p1!=NULL){free(p1): p1=NULL;}
//
// 形参不要随便更改，再定义一个指针，函数指针传递更改值是特色，注意传来的空指针判断。
// 函数出错立马判断，不要先是对的执行代码，会导致嵌套过多。
//
//
// const int a; = int const a; a不可更改，但可通过a的地址间接修改！
// const char *a;(*a值不可改) char * const a;(a不可改)
//
//
// 二维指针
// char *a[100]; 指针数组，每个元素都是指针。= char a[][100];
// 但前面的a是一个char*指针，后面是一个char数组指针
//
//
// 多维指针
// 多级指针的退化问题
