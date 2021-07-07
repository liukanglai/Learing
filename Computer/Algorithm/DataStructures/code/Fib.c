

// this is a loop 

#include<stdio.h>
int main(void)
{
    int f, f1, f2, n;
    f = f1 = f2 = n =0;
    scanf("%d",&n);
    for(int i=0;i<=n;i++){
        if(!i) f=f1=0;
        else if(i==1) f=f2=1;
        else {
            f=f1+f2;
            f1=f2;
            f2=f;
        }
    }
    printf("%d\n", f);
    return 0;
}



// use recursive


