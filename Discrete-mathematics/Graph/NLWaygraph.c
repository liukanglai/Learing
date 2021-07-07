#include<stdio.h>
#include<string.h>

int (*p)[4];

void Matrixmultiply(int (*a)[4],int (*p)[4], int n,int (*b)[4])
{
    int sum = 0;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            for(int m = 0;m < n;m++){
                    sum += a[i][m] * b[m][j];
            }
            *(*(p + i) + j) = sum;
            sum = 0;
        }
    }
}

void Nmultiply(int k, int (*a)[4],int (*b)[4], int n,int (*c)[4])
{
    p = c;
    for(int i = 1;i < k;i++){
        Matrixmultiply(a,p,n,b);
        memcpy(b,c,64);
    }
}

void CalculateElement(int (*p)[4],int n)
{
    int sum = 0;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            sum += p[i][j];
        }
    }
    printf("%d\n",sum);
}

int main(void)
{
    int a[4][4]={
        1,2,0,0,
        0,0,1,0,
        1,0,0,1,
        0,0,1,0
    }, b[4][4], n = 4,k;
    int c[4][4];
    memcpy(b,a,64);
    memcpy(c,a,64);
    k = 4;
    Nmultiply(k,a,b,n,c);
    CalculateElement(p,n);
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            printf("%d ",b[i][j]);
        }
    }
    return 0;
}
