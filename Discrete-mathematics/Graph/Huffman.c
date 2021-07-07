#include<stdio.h>
#include<stdlib.h>

int n;

int compare(const void* a,const void* b)
{
    return (*(int*)a - *(int*)b);
}


void select2(int *a,int n) //select already has.
{
    int flag = 0;//record the min,then let it be 10000
    int min1 = 10000,min2 = 10000;
    for(int i = 0;i < n;i++){
        if(a[i] < min1){ // can't use a[i] <= min,because the printf...
            min1 =a[i]; 
            flag = i;
        }
    }
    a[flag] = 10000;
    for(int i = 0;i < n;i++){
        if((a[i] < min2) && a[i] >= min1){
            min2 = a[i];
            flag = i;
        }
    }
    a[flag] = min1 + min2;
}

int main(void)
{
    printf("请输入权值个数:\n");
    scanf("%d",&n);
    int a[n];
    printf("请输入权值:\n");
    for(int i = 0;i < n;i++){
        scanf("%d",a+i);
    }
    qsort(a,n,sizeof(int),compare);
    for(int j = 0;j < n;j++){
        for(int i = 0;i < n;i++){
            if(a[i] < 10000) printf("%d\t",a[i]);
            else printf(" \t");
        }
        printf("\n");
        select2(a,n);
    }
    return 0;
}
