
// 2019011777 ¼ÆËã»ú19-3 Áõ¿µÀ´

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

//make the array large enough, One array stores positive numbers, the other stores negative numbers
int a[100000];
int b[100000];

//the array, record the entered value and it also has the function of sorting.

void Getline()
{
    int c;
    while (1) {
        scanf("%d", &c);
        if (c < 0) b[100000 + c] = 1;
        else a[c] = 1;
        if (getchar() == '\n') break;
    }
}

//from b to d,record the continuous value in array;if d - b > Maxline, write it; the flag can decide if the b should be called.

/*
int Getmax()
{
    int b,d,Maxline = 0,flag = 1;
    for(int i = 0;i <= 100000;i++){
        if(a[i] == 1){
            if(flag == 1){
                b = i;
                flag = 0;
            }
            if(a[i + 1] == 0 || i == 99999) {
                d = i + 1;
                flag = 1;
                Maxline = ((d - b) > Maxline) ?  (d - b) : Maxline;
            }
        }
    }
    return Maxline;
}
*/

int Getmax()
{
    int Maxline = 0;
    //b[i -1] should be valid.
    for (int i = 1; i < 100000; i++) {
        if (b[i] == 0) continue; // very important,
        b[i] += b[i - 1];
        Maxline = (b[i] > Maxline) ? b[i] : Maxline;
    }
    if (a[0]) a[0] += b[99999];
    for (int i = 1; i < 100000; i++) {
        if (a[i] == 0) continue; // very important,
        a[i] += a[i - 1];
        Maxline = (a[i] > Maxline) ? a[i] : Maxline;
    }
    return Maxline;
}

int main(void)
{
    printf("Please enter the int array:");
    Getline();
    printf("The result:%d\n", Getmax());
    system("ipconfig -all");
    system("pause");
    return 0;
}
