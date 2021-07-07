#include<stdio.h>

int Findway(int (*a)[4],int n,int i,int j,int *s)
{
    if(a[i][j] != 0){
        return 1;
    }
    else{
        s[i] = 1;
        for(int x = 0;x < n;x++){
            if(a[i][x] != 0 && s[x] == 0 && Findway(a,n,x,j,s) == 1) return 1;
        }
    }
    return 0;
}

int main(void)
{
    int a[4][4]={
        1,2,0,0,
        0,0,1,0,
        1,0,0,1,
        0,0,1,0
    }, b[4][4], n = 4;
    //s records the nodes that has been found.
    int s[4];
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            if(i == j) b[i][j] = 1;
            else b[i][j] = 0;
        }
    }
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            if(b[i][j] == 0 && Findway(a,n,i,j,s) == 1) b[i][j] = 1;
            // finish b[i][j],then s should be back.
            for(int c = 0;c < n;c++){
                s[c] = 0;
            }
        }
    }
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            printf("%d ",b[i][j]);
        }
        printf("\b\n");
    }
    return 0;
}
