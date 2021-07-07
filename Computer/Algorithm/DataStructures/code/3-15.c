#include<stdio.h>
#include <sys/types.h>
#define Maxsize 100


// 4 - m, n - 10

int m, n;
int q1[Maxsize];
int q2[Maxsize];
int q[n];
int l1 = 0, l2 = 0, endq = 0, startq = 0;

void enqueueq(int i){
    q[endq++] = i;
}

int outqueueq(){
    return q[startq++];
}

int main(void)
{
    int i = 0, j = 0, flag = 0, lq = 0;
    while(lq < n){
        if(j < l2 && (i >= l1 || (flag == m))){
            enqueueq(q2[j++]);
            if(flag == m){
                flag = 0;
            }
        }
        else if(i < l1){
            enqueueq(q1[i++]);
            flag++;
        }
        lq++;
    }

    return 0;
}
