#include<stdio.h>

int n,m;

int main(void)
{
    scanf("%d %d",&n,&m);
    struct Edge{
        int v1,v2,weight;
    }ex[m];
    for(int i = 0;i < m;i++){
        scanf("%d %d %d",&ex[i].v1,&ex[i].v2,&ex[i].weight);
    }
    // sort the ex by weight
    
    // the node that isn't in tree is 0
    // the length !
    int a[n+1]; 
    for(int i = 0;i < n+1;i++){
        a[i] = 0;
    }
    for(int i = 0;i < m;i++){
        if(ex[i].v1 ==ex[i].v2) continue; // delete ring
        if(a[ex[i].v1] && a[ex[i].v2] && a[ex[i].v1] == a[ex[i].v2]) continue;
        if(!a[ex[i].v1]){
            if(!a[ex[i].v2]) a[ex[i].v2] = a[ex[i].v1] = i+1; 
            else a[ex[i].v1] = a[ex[i].v2]; 
        }
        else{
            if(!a[ex[i].v2]) a[ex[i].v2] = a[ex[i].v1]; 
            else{
                //printf("%d %d ",a[5],a[7]);
                int s = a[ex[i].v2];
                for(int j = 0;j < n+1;j++){
                    if(a[j] == s) a[j] = a[ex[i].v1];
                }
            } 
        }
        //printf("%d %d\n",a[ex[i].v1],a[ex[i].v2]);
        printf("%d %d\n",ex[i].v1,ex[i].v2);
    }
    return 0;
}
