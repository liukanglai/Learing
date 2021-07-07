//calculate the sum of polynomicals: ai*pow(x,i)   i is from 1 to n.

#include<stdio.h>
int main(void)
{
    long double p=0,x; int n; float a,x0;
    scanf("%d %f",&n,&x0);
    for(int i = 0;i <= n;i++){
        scanf("%f",&a);
        if(!i) x = 1;
        else x *= x0;
        p+=a*x;
    }
    printf("%Lf/n",p);
    return 0;
}
