#include<stdio.h>

//make the array large enough
int a[100000];

//the array, record the entered value and it also has the function of sorting.

void Getline()
{
    int c;
    while(1){
        scanf("%d",&c);
        a[c]=1;
        if(getchar() == '\n') break;
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
    for(int i = 1;i < 100000;i++){
        if(a[i] == 0) continue; // very important,
        a[i] += a[i-1];
        if(a[i] > Maxline) Maxline = a[i];
    }
    return Maxline;
}

int main(void)
{
    Getline();
    printf("%d\n",Getmax());
    return 0;
}
