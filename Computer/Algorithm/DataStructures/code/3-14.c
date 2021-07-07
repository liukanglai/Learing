#include<stdio.h>
#define MaxSize 100 // n 的最大值

double Fnx(int n, double x)
{
    struct stack{
    int no; 
    double val ; // 保存Fn(x)值
    }S[MaxSize];
    int top = -1, i; // top 为栈st的下标值变量
    double fv1 = 1 , fv2 = 3 * x; // n=O, n=1的初值
    for (i = n; i >= 2; i--){
        top++;
        S[top].no = i; //入
    }
    while(top >= 0){
        S[top].val = 3 * x * fv2 - 3 * (S[top].no * S[top].no - S[top].no + 1 ) * fv1;
        fv1 = fv2;
        fv2 = S[top].val;
        top--; //出
    }
    if(n == 0){
        return fv1;
    }
    return fv2;
}



int main(void)
{
    
    return 0;

}
