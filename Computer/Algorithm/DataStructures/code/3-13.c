#include<stdlib.h>
#define Maxsize 100
 
int q1[Maxsize];
int q2[Maxsize];
int q[Maxsize];
int l1 = 0, l2 = 0, endq = 0, startq = 0;

void enqueueq(int i){
    if(endq == Maxsize){
        endq = 0;
    }
    q[endq++] = i;
}

int outqueueq(){
    if(startq == Maxsize){
        startq = 0;
    }
    return q[startq++];
}

int main()
{
	  int a[120];
	  int k, max;
	  scanf("%d%d", &k, &max);
	  int i;
	  for(i = 0; i < k-1; i++)
	  	a[i] = 0;
	  a[k-1] = 1;
	  a[k] = 1;

	  int n = k+1;

	  if(max == 0){
	      printf("%d\n", k-2);
	      printf("1\n");
	      return 0;
	  }
	  if(max == 1){
	      printf("%d\n", k);
	      printf("1\n");
	  	  return 0;
	  }
	  while(a[n-1] <= max){
	      a[n] = 2*a[n-1]-a[n-k-1];
	  	  n++;
	  }
    printf("%d\n", n-2);
	  printf("%d\n", a[n-2]);
	return 0;
}
