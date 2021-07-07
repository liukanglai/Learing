# 

T *p;

- p1 – p2 = ( 地址 p1 – 地址 p2 ) / sizeof( T)

- void 指针主要用于内存拷贝。源块和目的块的地址就都可以用 void 指针表示。C/C++中有以下标准库函数:
- void *memcpy(void *dest, const void *src, unsigned int n);
- 它在头文件 string.h 和 mem.h 中声明,作用就是将地址 src 开始的 n 字节内容,拷贝 到地址 dest。返回值就是 dest 。

        int a1[10];
        int a2[10];
        memcpy( a2, a1, 10*sizeof(int));


# 函数指针

        #include <stdio.h>
        void PrintMin(int a, int b) {
            if( a<b )
                printf("%d", a);
                else
            printf("%d", b);
        }
        int main(){
            void (* pf)(int , int);
            //定义函数指针 pf
            int x = 4, y = 5;
            pf = PrintMin;
            //用 PrintMin 函数对指针 pf 进行赋值
            pf(x, y);
            //调用 pf 指向的函数,即 PrintMin
            return 0;
        }

# qsort

```
void qsort(void *base, int nelem, unsigned int width, int ( * pfCompare)( const void *, const void *));

#include <stdio.h>
#include <stdlib.h>
int MyCompare( const void * elem1, const void * elem2 ) {
    unsigned int * p1, * p2;
    p1 = (unsigned int *) elem1;
    p2 = (unsigned int *) elem2;
    return (* p1 % 10) - (* p2 % 10 );
}
#define NUM 5
int main() {
    unsigned int an[NUM] = { 8,123,11,10,4 };
    qsort( an, NUM, sizeof(unsigned int), MyCompare);
    for( int i = 0;i < NUM; i ++ )
    printf("%d ", an[i]);
    return 0;
}
```
- 1) 如果 * elem1 应该排在 * elem2 前面,则函数返回值是负整数(任何负整数都行)
- 2) 如果 * elem1 和* elem2 哪个排在前面都行,那么函数返回 0
- 3) 如果 * elem1 应该排在 * elem2 后面,则函数返回值是正整数(任何正整数都行)

# wm

- 
