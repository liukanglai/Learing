# 


int * pn;
pn = new int; //(1)
* pn = 5;

int * pn;
int i = 5;
pn = new int[i * 20];
pn[0] = 20;

too big:
int * pn = new int[200000];
if( pn == NULL )
printf( “内存分配失败”);
else
printf( “内存分配成功”);

- delete必须是指向动态分配的内存空间的

int * p = new int;
* p = 5;
delete p;
delete p;
 //本句会导致程序异常

 int * p = new int[20];
p[0] = 1;
delete [] p;

