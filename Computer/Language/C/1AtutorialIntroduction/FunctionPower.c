#include<stdio.h>

int power(int m, int n); // declaration

// test power function

int main(void)
{
    int i;
    for(i = 0; i < 10; i++)
        printf("%d %d %d\n", i,power(2,i),power(-3,i));
    return 0;
}

// power: raise base to n-th power; n >= 0

int power(int base, int n)
{
    int i, p;

    p = 1;

    for(i = 1; i <= n; i++)
        p *= base;

    return p;
}

/*

// old-style version:

power(base, n)
int base, n;
{
    
}

// version 2(the value of n will not change):

int power(int base, int n)
{
    int p = 1;

    for(; n > 0; n--)
        p *= base;

    return p;
}

*/
