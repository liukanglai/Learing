// Create Graph

// 1. AdjacencyList 邻接表适用于稀疏图（边的数量远远小于顶点的数量）


typedef struct EdgeNode
{
    int toAdjVex;                               // The index of vertex array which this edge points to.
    float weight;                               // The edge weight.
    struct EdgeNode *next;                      // The next edge, note that it only means the next edge also links to the vertex which this edge links to.
}EdgeNode;

typedef struct VertexNode
{
    VERTEX_DATA_TYPE info;                      // The vertex info,.
    struct EdgeNode* firstEdge;                 // The first edge which the vertex points to.
}VertexNode;

typedef struct
{
    VertexNode adjList[VERTEX_NUM];             // Adjacency list, which stores the all vertexes of the graph.
    int vertextNum;                             // The number of vertex.
    int edgeNum;                                // The number of edge.
}AdjListGraph;

// 2. AdjacencyMatrix 邻接矩阵适用于稠密图（边的数量比较多），它的抽象描述如下： 上图是个无向无权图，仅用0、1来表示两个顶点是否相连，当然，通常做法是将边的权值来代替1，用一个十分大的数字（如∞）来代替0，表示不相连。

typedef struct 
{
    int number;
    VERTEX_DATA_TYPE info;
}Vertex;

typedef struct
{
    float edges[VERTEX_NUM][VERTEX_NUM];        // The value of this two dimensional array is the weight of the edge.
    int vertextNum;                             // The number of vertex.
    int edgeNum;                                // The number of edge.
    Vertex vex[VERTEX_NUM];                     // To store vertex.
}MGraph;

// 3. OrthogonalList

#define  MAX_VERTEX_NUM 20
#define  InfoType int  // 图中弧包含信息的数据类型
#define  VertexType int

typedef struct ArcBox{
    int tailvex,headvex; // 弧尾、弧头对应顶点在数组中的位置下标
    struct ArcBox *hlik,*tlink; // 分别指向弧头相同和弧尾相同的下一个弧
    InfoType *info; // 存储弧相关信息的指针
}ArcBox;

typedef struct VexNode{
    VertexType data;//顶点的数据域
    ArcBox *firstin,*firstout;//指向以该顶点为弧头和弧尾的链表首个结点
}VexNode;

typedef struct {
    VexNode xlist[MAX_VERTEX_NUM];//存储顶点的一维数组
    int vexnum,arcnum;//记录图的顶点数和弧数
}OLGraph;

int LocateVex(OLGraph * G,VertexType v){
    int i = 0;
    //遍历一维数组，找到变量v
    for (; i<G->vexnum; i++) {
        if (G->xlist[i].data==v) {
            break;
        }
    }
    //如果找不到，输出提示语句，返回 -1
    if (i > G->vexnum) {
        printf("no such vertex.\n");
        return -1;
    }
    return i;
}
//构建十字链表函数
void CreateDG(OLGraph *G){
    //输入有向图的顶点数和弧数
    scanf("%d,%d",&(G->vexnum),&(G->arcnum));
    //使用一维数组存储顶点数据，初始化指针域为NULL
    for (int i=0; i<G->vexnum; i++) {
        scanf("%d",&(G->xlist[i].data));
        G->xlist[i].firstin=NULL;
        G->xlist[i].firstout=NULL;
    }
    //构建十字链表
    for (int k=0;k<G->arcnum; k++) {
        int v1,v2;
        scanf("%d,%d",&v1,&v2);
        //确定v1、v2在数组中的位置下标
        int i=LocateVex(G, v1);
        int j=LocateVex(G, v2);
        //建立弧的结点
        ArcBox * p=(ArcBox*)malloc(sizeof(ArcBox));
        p->tailvex=i;
        p->headvex=j;
        //采用头插法插入新的p结点
        p->hlik=G->xlist[j].firstin;
        p->tlink=G->xlist[i].firstout;
        G->xlist[j].firstin=G->xlist[i].firstout=p;
    }
}

// 4. AdjacencyMulti-list

typedef enum {unvisited,visited}VisitIf;    //边标志域

typedef struct EBox{
    VisitIf mark;                           //标志域
    int ivex,jvex;                          //边两边顶点在数组中的位置下标
    struct EBox * ilink,*jlink;             //分别指向与ivex、jvex相关的下一个边
    InfoType *info;                         //边包含的其它的信息域的指针
}EBox;

typedef struct VexBox{
    VertexType data;                        //顶点数据域
    EBox * firstedge;                       //顶点相关的第一条边的指针域
}VexBox;

typedef struct {
    VexBox adjmulist[MAX_VERTEX_NUM];//存储图中顶点的数组
    int vexnum,degenum;//记录途中顶点个数和边个数的变量
}AMLGraph;

// Search

// 1. DFS深度优先搜索（depth-first search）

// adjlist

// recursion
void dfsRecursion(AdjListGraph* graph, int startVertexIndex, bool visit[])
{
    printf("%c ", (graph -> adjList[startVertexIndex]).info);
    visit[startVertexIndex] = true;
    EdgeNode* edgeIndex = (graph -> adjList[startVertexIndex]).firstEdge;
    while (edgeIndex != NULL)
    {
        if (visit[edgeIndex -> toAdjVex] == false)
            dfsRecursion(graph, edgeIndex -> toAdjVex, visit);
        edgeIndex = edgeIndex -> next;
    }
}

// stack
void dfsNonRecursion(AdjListGraph* graph, int startVertextIndex, bool visit[])
{
    linked_stack* stack = NULL;
    init_stack(&stack);

    // Visit the start vertex.
    printf("%c ", (graph -> adjList[startVertextIndex]).info);
    visit[startVertextIndex] = true;
    EdgeNode* edgeNode = (graph -> adjList[startVertextIndex]).firstEdge;
    if (edgeNode != NULL)
        push(stack, edgeNode);
    while (!isEmptyStack(stack))
    {
        edgeNode = ((EdgeNode*)pop(stack)) -> next;
        while (edgeNode != NULL && !visit[edgeNode -> toAdjVex])
        {
            printf("%c ", (graph -> adjList[edgeNode -> toAdjVex]).info);
            visit[edgeNode -> toAdjVex] = true;
            push(stack, edgeNode);
            edgeNode = (graph -> adjList[edgeNode -> toAdjVex]).firstEdge;
        }
    }
}


// 邻接矩阵


// 邻接矩阵结构（无向图）
typedef struct {
    VertexType vexs[MAXVEX]; // 顶点表
    EdgeType arc[MAXVEX][MAXVEX]; // 边表
    int numNodes, numEdges; // 图的顶点数、边数
} MGraph;

// 访问标志的数组
int visited[MAXVEX];

void DFS(MGraph G, int i) {
    int j; // 用于遍历元素

    visited[i] = TRUE; // 记录该下标的元素已被访问

    printf("%c ", G.vexs[i]); // 打印该位置的顶点值

    // 遍历图中的顶点
    for (j = 0; j < G.numNodes; j++) {
        // 顶点i到顶点j有边相连，并且顶点j未被访问过
        if (G.arc[i][j] == 1 && !visited[j]) {
            DFS(G, j); // 对顶点j进行访问
        }
    }
}

void DFSTraverse(MGraph G) {
    int i; // 用于遍历元素

    // 初始化设置所有顶点都没被访问过
    for (i = 0; i < G.numNodes; i++) {
        visited[i] = FALSE;
    }

    // 遍历顶点i
    for (i = 0; i < G.numNodes; i++) {
        // 如果顶点i未被访问过
        if (!visited[i]) {
            DFS(G, i); // 访问顶点i
        }
    }
}



// 2. BFS

void BFS(AdjListGraph* graph, int startVertexIndex, bool visit[])
{
    // Loop queue initialization.
    LoopQueue loopQ;
    loopQ.front = 0;
    loopQ.rear = 0;
    LoopQueue* loopQueue = &loopQ;
    enqueue(loopQueue, &(graph -> adjList[startVertexIndex]));
    printf("%c ", (graph -> adjList[startVertexIndex]).info);
    visit[startVertexIndex] = true;
    while (!isEmpty(loopQueue))
    {
        VertexNode* vertexNode = dequeue(loopQueue);
        EdgeNode* edgeNode = vertexNode -> firstEdge;
        while(edgeNode != NULL)
        {
            if (visit[edgeNode -> toAdjVex] == false)
            {
                printf("%c ", (graph -> adjList[edgeNode -> toAdjVex]).info);
                visit[edgeNode -> toAdjVex] = true;
                enqueue(loopQueue, &(graph -> adjList[edgeNode -> toAdjVex]));
            }
            edgeNode = edgeNode -> next;
        }
    }
}




// 矩阵
//
// 循环队列顺序存储结构
typedef struct {
    int data[MAXSIZE]; // 用于存值的数组
    int front; // 头指针
    int rear; // 尾指针，若队列不空，指向队尾元素的下一个位置
}Queue;

int InitQueue(Queue *Q) {
    Q->front = Q->rear=  0; // 队头和队尾指针都指向0
    return 1;
}

int QueueEmpty(Queue Q) {
    if (Q.front == Q.rear) { // 队头等于队尾指针，队列为空
        return TRUE;
    } else {
        return FALSE;
    }
}

Status EnQueue(Queue *Q, int e) {
    // 队列已满，插入失败
    if ((Q->rear + 1) % MAXSIZE == Q->front) {
        return ERROR;
    }

    // 将元素e插入队尾
    Q->data[Q->rear] = e;

    // 设置队尾指针指向下一个位置，若到最后则转向头部
    Q->rear = (Q->rear + 1) % MAXSIZE;
    return OK;
}

int DeQueue(Queue *Q, int *e) {
    // 对头指针等于对尾指针，此时队列为空，出队失败
    if (Q->front == Q->rear) {
        return 0;
    }

    // 将队头元素的值赋给元素e
    *e = Q->data[Q->front];

    // 设置队头指针指向下一个位置，若到最后则转向头部
    Q->front = (Q->front + 1) % MAXSIZE;
    return 1;
}


void BFSTraverse(MGraph G) {
    int i, j; // 用于遍历元素
    Queue Q; // 队列

    // 初始设置图的所有顶点都没被访问过
    for (i = 0; i < G.numNodes; i++) {
        visited[i] = FALSE;
    }

    InitQueue(&Q); // 初始化队列

    // 对每一个顶点做循环
    for (i = 0; i < G.numNodes; i++) {
        if (!visited[i]) { // 该顶点未被访问过，进行处理
            visited[i] = TRUE; // 设置该顶点i已被访问

            printf("%c ", G.vexs[i]); // 打印该顶点i的值

            EnQueue(&Q, i); // 将该顶点i入队

            // 当队列非空时，进行循环
            while (!QueueEmpty(Q)) {
                DeQueue(&Q, &i); // 将队头元素出队，赋值给i

                // 遍历当前节点以外的节点j
                for (j = 0; j < G.numNodes; j++) {
                    // 若顶点j与当前节点存在边，并且未被访问过
                    if (G.arc[i][j] == 1 && !visited[j]) {
                        visited[j] = TRUE; // 设置顶点j已被访问
                        printf("%c ", G.vexs[j]); // 打印顶点j的值

                        EnQueue(&Q, j); // 将顶点j入队
                    }
                }
            }
        }
    }
}

// Minimum Spanning Tree

// 1. Prim

float prim(MGraph* graph, int startVertex)
{
    float totalCost = 0;
    float lowCost[VERTEX_NUM];              // The value of lowCost[i] represents the minimum distance from vertex i to current spanning tree.
    bool treeSet[VERTEX_NUM];               // The value of treeSet[i] represents whether the vertex i has been merged into the spanning tree.

    // Initialization
    for (int i = 0; i < (graph -> vertextNum); i++)
    {
        lowCost[i] = graph -> edges[startVertex][i];        // Init all cost from i to startVertex.
        treeSet[i] = false;                                 // No vertex is in the spanning tree set at first.
    }

    treeSet[startVertex] = true;                            // Merge the startVertex into the spanning tree set.
    printf("%c ", (graph -> vex[startVertex]).info);
    for (int i = 0; i < (graph -> vertextNum); i++)
    {
        int minCost = MAX_COST;                             // MAX_COST is a value greater than any other edge weight.
        int newVertex = startVertex;

        // Find the minimum cost vertex which is out of the spanning tree set.
        for (int j = 0; j < (graph -> vertextNum); j++)
        {
            if (!treeSet[j] && lowCost[j] < minCost)
            {
                minCost = lowCost[j];
                newVertex = j;
            }
        }
        treeSet[newVertex] = true;                          // Merge the new vertex into the spanning tree set.

        /*

            Some ops, for example you can print the vertex so you will get the sequence of node of minimum spanning tree.

        */
        if (newVertex != startVertex)
        {
            printf("%c ", (graph -> vex[newVertex]).info);
            totalCost += lowCost[newVertex];
        }

        // Judge whether the cost is change between the new spanning tree and the remaining vertex.
        for (int j = 0; j < (graph -> vertextNum); j++)
        {
            if (!treeSet[j] && lowCost[j] > graph -> edges[newVertex][j])
                lowCost[j] = graph -> edges[newVertex][j];  // Update the cost between the spanning tree and the vertex j.
        }
    }
    return totalCost;
}


// 2. Kruskal's algorithm 并查集 （Union Find Set）

int findRootInSet(int array[], int x)
{
    if (array[x] < 0)
    {
        // Find the root index.
        return x;
    }
    else
    {
        // Recursively find its parent until find the root,
        // then recursively update the children node so that they will point to the root.
        return array[x] = findRootInSet(array, array[x]);
    }
}

bool unionSet(int array[], int node1, int node2)
{
    int root1 = findRootInSet(array, node1);
    int root2 = findRootInSet(array, node2);
    if (root1 == root2)
    {
        // It means they are in the same set
        return false;
    }

    // The value of array[root] is negative and the absolute value is its children numbers,
    // when merging two sets, we choose to merge the more children set into the less one.
    if (array[root1] > array[root2])
    {
        array[root1] += array[root2];
        array[root2] = root1;
    }
    else
    {
        array[root2] += array[root1];
        array[root1] = root2;
    }
    return true;
}

void Kruskal()
{
    int array[n];
	  /* 将边排序 */
	  qsort(e, n, sizeof(edge), cmp);

	  for (i = 0; i < n; i++)
	  {
	  	x = findRootInSet(array, e[i].x);
	  	y = findRootInSet(array, e[i].y);
	  	if (x != y || (!x && !y))
	  	{
	  		printf("%c - %c : %d\n", e[i].x, e[i].y, e[i].w);
	  		Union(x, y, e[i].w);
	  	}
	  }
}


// 1. dijkstra
void dijkstra(MGraph* graph, int startVertexIndex)
{
    // For storing the minimum cost from the arbitrary node to the start vertex.
    float minCostToStart[VERTEX_NUM];
    // For marking whether the node is in the set.
    int set[VERTEX_NUM];
    // Initialization
    for(int i = 0; i < VERTEX_NUM; i++) {
        minCostToStart[i] = graph->edges[i][startVertexIndex];
        set[i] = 0;
    }
    // Add the start vertex into the set.
    set[startVertexIndex] = 1;
    int minNodeIndex = startVertexIndex;
    for (int count = 1; count < VERTEX_NUM; count++){
        int minCost = MAX_COST;
        // Find the adjacent node which is nearest to the startVertexIndex.
        for (int i = 0; i < VERTEX_NUM; i++) {
            if (!set[i] && minCostToStart[i] < minCost)
            {
                minCost = minCostToStart[minNodeIndex];
                minNodeIndex = i;
            }
        }
        // Add the proper node into the set
        set[minNodeIndex] = 1;
        // After the new node is added into the set, update the minimum cost of each node which is out of the set.
        for (int i = 0; i < VERTEX_NUM; i++) {
            if (!set[i] && (graph -> edges[i][minNodeIndex]) < MAX_COST) {
                // The new cost of each node to source = the cost of new added node to source + the cost of node i to new added node.
                float newCost = minCostToStart[minNodeIndex] + graph -> edges[i][minNodeIndex];
                if (newCost < minCostToStart[i])
                    minCostToStart[i] = newCost;
            }
        }
    }
    printf("The cost of %c to each node:\n", (graph -> vex[startVertexIndex]).info);
    for (int i = 0; i < VERTEX_NUM; i++) {
        if (i != startVertexIndex)
            printf("-----> %c : %f\n", (graph -> vex[i]).info, minCostToStart[i]);
    }
}


//2. 多源最短路径算法——弗洛伊德算法（Floyd Algorithm）

void floyd(MGraph* graph)
{
    float minCost[VERTEX_NUM][VERTEX_NUM];          // Store the distance between any two nodes.
    int path[VERTEX_NUM][VERTEX_NUM];               // Store the intermediate node between the two nodes.
    int i, j, k;

    // Initialization
    for (i = 0; i < VERTEX_NUM; i++)
    {
        for (j = 0; j < VERTEX_NUM; j++)
        {
            minCost[i][j] = graph -> edges[i][j];
            path[i][j] = -1;
        }
    }

    // Find if there is another k node, it makes the distance dis[i][k] + dis[k][j] < dis[i][j];
    for (k = 0; k < VERTEX_NUM; k++)
        for (i = 0; i < VERTEX_NUM; i++)
            for (j = 0; j < VERTEX_NUM; j++)
            {
                if (minCost[i][j] > minCost[i][k] + minCost[k][j])
                {
                    minCost[i][j] = minCost[i][k] + minCost[k][j];
                    path[i][j] = k;
                }
            }

    for (i = 0; i < VERTEX_NUM; i++)
        for (j = 0; j < VERTEX_NUM; j++)
        {
            if (i != j && minCost[i][j] != MAX_COST)
                printf("%c ---> %c, the minimum cost is %f\n", (graph -> vex[i]).info, (graph -> vex[j]).info, minCost[i][j]);
        }
}

// 拓扑排序（topological sort）

// 顶点表示活动的网（activity on vertex network）或AOV网，


对于拓扑排序问题，所需的操作主要有：
1）判断顶点是否有前驱；
2）删除顶点和关联于该顶点的所有边。

for(i=0;i<n;i++)
{
    if every vertex has a predecessor
    {
        fprintf(stderr,"Network has a cycle.\n");
        exit(1);
    }
    pick a vertex v that has no predecessors;
    output v;
    delete v and all edges leading out of v from the netwok;
}

//  邻接表
typedef struct node
{
    int vertex;
    struct node *link;
}node;
typedef struct
{
    int count; // 有几个前边 
    struct node * link;
}hdnodes;

hdnodes graph[MAX_VERTICES];

// 函数
void topsort(hdnodes graph[],int n)
{
    int i,j,k,top;
    node *ptr;
    top=-1;
    // find the first top.
    for(i=0;i<n;i++)
    {
        if(!graph[i].count)
        {
            graph[i].count=top;
            top=i;
        }
    }
    for(i=0;i<n;i++)
    {
        if(top==-1)
        {
            fprintf(stderr,"\nNetwork has a cycle. Sort terminated.\n");
            return ;
        }
        else
        {
            j=top;
            top=graph[top].count;
            printf("%d, ",j);
            for(ptr=graph[j].link; ptr; ptr=ptr->link)
            {
                k=ptr->vertex;
                graph[k].count--;
                if(!graph[k].count)
                {
                    graph[k].count=top;
                    top=k;
                }
            }
        }
    }
}


typedef struct
{
	char vexs[N];//顶点数组
	int arcs[N][N];//邻接矩阵
}graph;

void TopoSort_matrix(graph g)
{
	int row[N]={0};//按照列来设置标志，为1表示已经输出（不再考虑），为0表示未输出。
	int v=1;//标志符，1表示已经输出（不再考虑），为0表示未输出，赋给row数组
	int i,j,k,t,m;
	for(k=0;k<N;k++)
	{
		for(j=0;j<N;j++)
		{
			if(row[j]==0)//活动j还未输出
			{
				t=1; //标识符
				for(i=0;i<N;i++)
					if(g.arcs[i][j]==1) //当前活动有入度（活动i必须在活动j之前）
					{
						t=0;
						break;
					}
				if(t==1)/ /活动j没有入度
				{
					m=j;
					break;
				}
			}
		}
		if(j!=N)
		{
			row[m]=v;
			printf("%c",g.vexs[m]);
			for(i=0;i<N;i++)
				g.arcs[m][i]=0; //将已经输出的活动所到达的下个活动的入度置为0
			v++;
		}
		else
			break;
		}
		if(v-1 < N)//当row中不是所有的元素都被赋予新值v时，说明有环存在
			printf("\n该有向图有环存在！");

}

// AOE网络就是边表示活动的网络（activity on edge network）关键路径（critical path）
//
// CriticalPath

#include <stdio.h>
#include <stdlib.h>
#define  MAX_VERTEX_NUM 20//最大顶点个数
#define  VertexType int//顶点数据的类型

typedef enum{false,true} bool;

//建立全局变量，保存边的最早开始时间
VertexType ve[MAX_VERTEX_NUM];
//建立全局变量，保存边的最晚开始时间
VertexType vl[MAX_VERTEX_NUM];

typedef struct ArcNode{
    int adjvex;//邻接点在数组中的位置下标
    struct ArcNode * nextarc;//指向下一个邻接点的指针
    VertexType dut;
}ArcNode;

typedef struct VNode{
    VertexType data;//顶点的数据域
    ArcNode * firstarc;//指向邻接点的指针
}VNode,AdjList[MAX_VERTEX_NUM];//存储各链表头结点的数组

typedef struct {
    AdjList vertices;//图中顶点及各邻接点数组
    int vexnum,arcnum;//记录图中顶点数和边或弧数
}ALGraph;

ALGraph *G;

//找到顶点对应在邻接表数组中的位置下标
int LocateVex(ALGraph G,VertexType u){
    for (int i=0; i<G.vexnum; i++) {
        if (G.vertices[i].data==u) {
            return i;
        }
    }
    return -1;
}

//创建AOE网，构建邻接表
void CreateAOE(ALGraph **G){
    *G=(ALGraph*)malloc(sizeof(ALGraph));
   
    scanf("%d,%d",&((*G)->vexnum),&((*G)->arcnum));
    for (int i=0; i<(*G)->vexnum; i++) {
        scanf("%d",&((*G)->vertices[i].data));
        (*G)->vertices[i].firstarc=NULL;
    }
    VertexType initial,end,dut;
    for (int i=0; i<(*G)->arcnum; i++) {
        scanf("%d,%d,%d",&initial,&end,&dut);
       
        ArcNode *p=(ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex=LocateVex(*(*G), end);
        p->nextarc=NULL;
        p->dut=dut;
       
        int locate=LocateVex(*(*G), initial);
        p->nextarc=(*G)->vertices[locate].firstarc;
        (*G)->vertices[locate].firstarc=p;
    }
}

//结构体定义栈结构
typedef struct stack{
    VertexType data;
    struct stack * next;
}stack;
stack *T;

//初始化栈结构
void initStack(stack* *S){
    (*S)=(stack*)malloc(sizeof(stack));
    (*S)->next=NULL;
}

//判断栈是否为空
bool StackEmpty(stack S){
    if (S.next==NULL) {
        return true;
    }
    return false;
}
//进栈，以头插法将新结点插入到链表中
void push(stack *S,VertexType u){
    stack *p=(stack*)malloc(sizeof(stack));
    p->data=u;
    p->next=NULL;
    p->next=S->next;
    S->next=p;
}
//弹栈函数，删除链表首元结点的同时，释放该空间，并将该结点中的数据域通过地址传值给变量i;
void pop(stack *S,VertexType *i){
    stack *p=S->next;
    *i=p->data;
    S->next=S->next->next;
    free(p);
}
//统计各顶点的入度
void FindInDegree(ALGraph G,int indegree[]){
    //初始化数组，默认初始值全部为0
    for (int i=0; i<G.vexnum; i++) {
        indegree[i]=0;
    }
    //遍历邻接表，根据各链表中结点的数据域存储的各顶点位置下标，在indegree数组相应位置+1
    for (int i=0; i<G.vexnum; i++) {
        ArcNode *p=G.vertices[i].firstarc;
        while (p) {
            indegree[p->adjvex]++;
            p=p->nextarc;
        }
    }
}
bool TopologicalOrder(ALGraph G){
    int indegree[G.vexnum];//创建记录各顶点入度的数组
    FindInDegree(G,indegree);//统计各顶点的入度
    //建立栈结构，程序中使用的是链表
    stack *S;
    //初始化栈
    initStack(&S);
    for (int i=0; i<G.vexnum; i++) {
        ve[i]=0;
    }
    //查找度为0的顶点，作为起始点
    for (int i=0; i<G.vexnum; i++) {
        if (!indegree[i]) {
            push(S, i);
        }
    }
    int count=0;
    //栈为空为结束标志
    while (!StackEmpty(*S)) {
        int index;
        //弹栈，并记录栈中保存的顶点所在邻接表数组中的位置
        pop(S,&index);
        //压栈，为求各边的最晚开始时间做准备
        push(T, index);
        ++count;
        //依次查找跟该顶点相链接的顶点，如果初始入度为1，当删除前一个顶点后，该顶点入度为0
        for (ArcNode *p=G.vertices[index].firstarc; p ; p=p->nextarc) {
           
            VertexType k=p->adjvex;
           
            if (!(--indegree[k])) {
                //顶点入度为0，入栈
                push(S, k);
            }
            //如果边的源点的最长路径长度加上边的权值比汇点的最长路径长度还长，就覆盖ve数组中对应位置的值，最终结束时，ve数组中存储的就是各顶点的最长路径长度。
            if (ve[index]+p->dut>ve[k]) {
                ve[k]=ve[index] + p->dut;
            }
        }
    }
    //如果count值小于顶点数量，表明有向图有环
    if (count<G.vexnum) {
        printf("该图有回路");
        return false;
    }
    return true;
}
//求各顶点的最晚发生时间并计算出各边的最早和最晚开始时间
void CriticalPath(ALGraph G){
    if (!TopologicalOrder(G)) {
        return ;
    }
    for (int i=0 ; i<G.vexnum ; i++) {
        vl[i]=ve[G.vexnum-1];
    }
    int j,k;
    while (!StackEmpty(*T)) {
        pop(T, &j);
        for (ArcNode* p=G.vertices[j].firstarc ; p ; p=p->nextarc) {
            k=p->adjvex;
            //构建Vl数组，在初始化时，Vl数组中每个单元都是18，如果每个边的汇点-边的权值比源点值小，就保存更小的。
            if (vl[k]-p->dut<vl[j]) {
                vl[j] = vl[k] - p->dut;
            }
        }
    }
    for (j = 0; j < G.vexnum; j++) {
        for (ArcNode*p = G.vertices[j].firstarc; p ;p = p->nextarc) {
            k = p->adjvex;
            //求各边的最早开始时间e[i],等于ve数组中相应源点存储的值
            int ee = ve[j];
            //求各边的最晚开始时间l[i]，等于汇点在vl数组中存储的值减改边的权值
            int el = vl[k]-p->dut;
            //判断e[i]和l[i]是否相等，如果相等，该边就是关键活动，相应的用*标记；反之，边后边没标记
            char tag = (ee==el)?'*':' ';
            printf("%3d%3d%3d%3d%3d%2c\n",j,k,p->dut,ee,el,tag);
        }
    }
}
