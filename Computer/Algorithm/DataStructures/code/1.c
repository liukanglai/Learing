#include<stdio.h>
#include<stdlib.h>
#define VERTEX_NUM 26
#define Maxsize 1000

int K, N; // K, N < 5
int startnode, endnode;

int perm[Maxsize][Maxsize], permN = 0;
void swap(int* a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void Permutations(int str[], int index, int str_size)
{
    int i = 0,j = 0;

    if(index == str_size)
    {
        for(i = 0; i < str_size; i++){
            perm[permN][i] = str[i];
        }
        permN++;
    }
    else
    {
        for(j = index;j < str_size; j++)
        {
            swap(&str[j],&str[index]);
            Permutations(str,index+1,str_size);
            swap(&str[j],&str[index]);
        }
    }
}

typedef struct
{
    int edges[VERTEX_NUM][VERTEX_NUM];        // The value of this two dimensional array is the weight of the edge.
    int vertextNum;                             // The number of vertex.
    int edgeNum;                                // The number of edge.
    int vex[VERTEX_NUM];                     // To store vertex.
}MGraph;

int dijkstra(MGraph* G, int startVertexIndex, int endVertexIndex, int p1[], int *Npath)
{
    int d[VERTEX_NUM]; // 记录v0到各定点的最小路径
    int use[VERTEX_NUM]; // 记录各节点是否已求得最短路径，0表示未求得，1表示已求得
    int p[VERTEX_NUM]; // 记录v0到个顶点最小路径的中间节点
    for(int i = 0; i < VERTEX_NUM; i++){
        use[i] = 0;
        p[i] = startVertexIndex;
        d[i] = G->edges[startVertexIndex][i];
    }
    use[startVertexIndex] = 1; //由于v0-v0不存在，因此当v0已求得最短路径

    for(int i = 1; i < VERTEX_NUM; i++){
        if(use[endVertexIndex])
            break;
        int s, min = 1000;// s为中间点,min为最小路径
        for(int j = 0; j < VERTEX_NUM; j++){ // 找到最短的路径
            if(!use[j] && d[j] < min){
                min = d[j];
                s = j;
            }
        }
        use[s] = 1; // 标记此点已过
        for(int w = 0; w < VERTEX_NUM; w++){ // 重新标记路径
            if(!use[w] && (min + G->edges[s][w] < d[w])){
                d[w] = min + G->edges[s][w];
                p[w] = s;
            }
        }
    }
    if(!use[endVertexIndex]){
        *Npath = 0;
        return 1000;
    }
    // 将Path赋值
    int w, p2[26], i = 0;
    p2[0] = endVertexIndex;
    int z = endVertexIndex;
    for(w = 1; w < VERTEX_NUM; w++){ // 此处隐含所经路径节点不超过总节点数。
        p2[w] = p[z];
        z = p[z];
        if(z == startVertexIndex)
            break;
    }
    *Npath = w+1;
    while(w >= 0){
        p1[i++] = p2[w--];
    }
    return d[endVertexIndex];
}

/*
//深度优先递归算法
void DFS(MGraph *G, int i, int visited[])
{
    if(!visited[i])
        return ;
	  visited[i] = 1;   //被访问的标记
	  //printf("%c->",G.vexs[i]);
	  for(int j = 0; j < G->vertextNum; j++) {
	  	if(G->edges[i][j] < 1000 && !visited[j])   //边(i,j)存在且j顶点未被访问，递归
	  		DFS(G, j, visited);
	  }
}

int isConnected(int p[], MGraph *G)
{
    int root = p[0];
    int visited[26];
	  for(int i = 0; i < G->vertextNum; i++)
		    visited[i] = 0;
    DFS(G, root, visited);
	  for(int i = 0; i < K; i++){
	      if(!visited[p[i]])
	          return 0;
	  }
	  return 1;
}
*/
 
void FindKpaths(MGraph *G, int Paths[], int Dist[], int Npaths[])
{
    int Path1[VERTEX_NUM], Path2[VERTEX_NUM], Path22[VERTEX_NUM], Path3[VERTEX_NUM];
    int Npath1, Npath2 = 0, Npath22, Npath3;
    int *a = &Npath1, *b = &Npath22, *c = &Npath3;
    int dist1, dist2 = 0, dist22, dist3, dist;
    //if(!(isConnected(perm[0], G)))
     //   return ;
    if(!K){
        Dist[0] = dijkstra(G, startnode, endnode, Paths, Npaths);
        return;
    }
    for(int i = 0; i < permN; i++){
        if(K == 1){
            dist2 = 0;
            Npath2 = 0;
        }
        dist2 = 0;
        Npath2 = 0;
        for(int j = 0; j < K-1; j++){
            dist22 = dijkstra(G, perm[i][j], perm[i][j+1], Path22, b);
            dist2 += dist22;
            if(!j)
                for(int m = 0; m < Npath22;){
                    Path2[Npath2++] = Path22[m++];
                }
            else
                for(int m = 1; m < Npath22;){
                    Path2[Npath2++] = Path22[m++];
                }
        }
        dist1 = dijkstra(G, startnode, perm[i][0], Path1, a);
        dist3 = dijkstra(G, perm[i][K-1], endnode, Path3, c);
        dist = dist1 + dist2 + dist3;
        if(dist < Dist[0]){
            Dist[0] = dist;
            int s = 0;
            for(int k = 0; k < Npath1; ){
                Paths[s++] = Path1[k++];
            }
            for(int k = 1; k < Npath2; ){
                Paths[s++] = Path2[k++];
            }
            for(int k = 1; k < Npath3; ){
                Paths[s++] = Path3[k++];
            }
            Npaths[0] = s;
        }
    }
}

void FindNpaths(MGraph *G, int Paths[][VERTEX_NUM], int Dist[], int Npaths[])
{
    int P[26], Np, *np = &Np, dist[26];
    int CutEdgeSet[26][26], CutEdgeNum[26]; 
    int EdgeNum = 0;
    int i, j;
    int edge, edgedist, flagContinue;
    
    for(i = 1; i < N; i++){ // 初始化
        for(j = 0; j < VERTEX_NUM; j++){ 
            CutEdgeSet[0][j] = -1;
            CutEdgeSet[i][j] = -1;
            Paths[i][j] = -1;
        }
        Npaths[i] = 0;
        dist[i] = -1;
        CutEdgeNum[i] = 0;
    }
    CutEdgeNum[0] = 0;
    for(i = 1; i < N ; i++){
        if(Npaths[i-1] == 0) break; //上一条最短路径不存在 , 结束运行
        for(j = 0; j < CutEdgeNum[i-1]; j++){ // 在G 中将 CutEdgeSet中各边删除 , 得到子图 G′
            edge = CutEdgeSet[i-1][j];
            dist[j] = G->edges[edge/100][edge%100];
            G->edges[edge/100][edge%100] = 1000;
        }
        // 在G′中依次删除Paths[i-1]中的各边,求G′的最短路径上一条最短路径的边数
        EdgeNum = Npaths[i-1] - 1; // 边数为顶点数-1
        for(j = 0; j < EdgeNum; j++){ 
            // 找到要断开的边, 如是1022，表示是10顶点与22，而120，表示1与20顶点。。。小顶点一定在前，例如：121 是边1与21 还是 12与1.
            if(Paths[i-1][j] <= Paths[i-1][j+1])
                edge = Paths[i-1][j] * 100 + Paths[i-1][j+1];
            else
                edge = Paths[i-1][j+1] * 100 + Paths[i-1][j];
            /*
             //新删除边集合与已有删除边集合是否有相同的标志 , 若相同则为 false
            flagContinue = 1;
            // 若 edge ∪ CutEdgeSet与已有删除边集合相同, 则没有必要继续进行下面的运算, 因为得到的最短路径是一样的
            //if(CutEdgeSetCompare(edge ∪ CutEdgeSet, jj) == 1){
            for(int m = 1; m < 26; m++){
                flagContinue = 0;
                for(int n = 0; n < CutEdgeNum[m]; n++){
                    if(CutEdgeSet[i-1][n] != CutEdgeSet[m][n]){
                        flagContinue = 1;
                        break;
                    }
                }
            }
            if(!flagContinue)
                break;
            */
            for(int m = 0; m < Np; m++){
                P[m] = -1;
            }
            edgedist = G->edges[edge/100][edge%100]; // 存储边值
            G->edges[edge/100][edge%100] = 1000; // 断开边
            int dist1; // 暂存路径的值
            dist1 = dijkstra(G, startnode, endnode, P, np);
            G->edges[edge/100][edge%100] = edgedist; // 还原边
            for(int k = i; k < N; k++){
                if(dist1 <= Dist[k] && dist1 != 1000){ // 找所有子图最短路径中的最短的
                    //路径P尚未出现过
                    int flag = 1;
                    for(int m = 0; m < N; m++){
                        flag = 1;
                        for(int n = 0; n < Np; n++){
                            if(P[n] != Paths[m][n]){
                                flag = 0;
                                break;
                            }
                        }
                        if(flag == 1)
                            break;
                    }
                    if(Npaths[i] && flag)
                        break;
                    for(int m = N - 1; m > k; m--){ //将路径P插入适当位置 , 使 Path s[i] 中各条路径按升序存放
                        for(int n = 0; n < Npaths[m-1]; n++){
                            Paths[m][n] = Paths[m-1][n];  // 将在上一个子图中找到的路径合理利用，减少时间
                            CutEdgeSet[m][n] = CutEdgeSet[m-1][n];
                        }
                        CutEdgeNum[m] = CutEdgeNum[m-1];
                        Npaths[m] = Npaths[m-1];
                        Dist[m] = Dist[m-1];
                    }
                    for(int n = 0; n < CutEdgeNum[k-1]; n++)
                        CutEdgeSet[k][n] = CutEdgeSet[k-1][n];
                    Npaths[k] = Np;
                    for(int m = 0; m < Npaths[k]; m++)
                        Paths[k][m] = P[m]; 
                    CutEdgeNum[k] = CutEdgeNum[k-1] + 1;
                    CutEdgeSet[k][CutEdgeNum[k-1]] = edge;
                    Dist[k] = dist1;
                    break;
                }
            }
        }
        for(j = 0; j < CutEdgeNum[i-1]; j++){  // 恢复图 
            edge = CutEdgeSet[i-1][j]; 
            G->edges[edge/100][edge%100] = dist[j]; // 还原边
            //edge.dist = 1000;// 原算法的edge.dist = ∞ 是不正确的
        }
    }
}


int main(void)
{    
    MGraph *G = (MGraph *)malloc(sizeof(MGraph));
    //初始化图
    /*
    printf("Please input the number of the vertexs:");
    scanf("%d", &G->vertextNum);
    printf("Please input the number of the edges:");
    scanf("%d", &G->edgeNum);
    for(int i = 0; i < VERTEX_NUM; i++){
        G->vex[i] = i;
    }
    for(int i = 0; i < VERTEX_NUM; i++){
        for(int j = 0; j < VERTEX_NUM; j++){
            scanf("%d", G->edges[i][j]);
        }
    }
    */
    G->vertextNum = 26;
    G->edgeNum = 100; // 估计。。。
    for(int i = 0; i < VERTEX_NUM; i++){
        G->vex[i] = i+1;
    }
    for(int i = 0; i < VERTEX_NUM; i++){
        for(int j = 0; j < VERTEX_NUM; j++){
            G->edges[i][j] = 1000;
        }
    }
    G->edges[0][1] = 62; G->edges[0][4] = 37; G->edges[0][5] = 16; G->edges[0][6] = 23; G->edges[0][8] = 37;
    G->edges[1][2] = 25; G->edges[1][3] = 18; G->edges[1][4] = 34; 
    G->edges[2][3] = 26; G->edges[2][13] = 32; G->edges[2][14] = 20;
    G->edges[3][4] = 19; G->edges[3][11] = 32; G->edges[3][12] = 31; G->edges[3][14] = 17;
    G->edges[4][8] = 13; G->edges[4][11] = 27; G->edges[4][12] = 35;
    G->edges[5][6] = 10; G->edges[5][7] = 22;
    G->edges[6][7] = 17; G->edges[6][8] = 27; G->edges[6][10] = 33;
    G->edges[7][9] = 25; G->edges[7][10] = 40; G->edges[7][15] = 41;
    G->edges[8][10] = 16; G->edges[8][11] = 22;
    G->edges[9][10] = 28; G->edges[9][15] = 21; G->edges[9][16] = 15; G->edges[9][17] = 38;
    G->edges[10][17] = 25;
    G->edges[11][12] = 19; G->edges[11][16] = 45; G->edges[11][18] = 25; G->edges[11][22] = 42;
    G->edges[12][13] = 25; G->edges[12][14] = 17; G->edges[12][18] = 20;
    G->edges[13][14] = 22; G->edges[13][18] = 39; G->edges[13][19] = 18;
    G->edges[14][19] = 38;
    G->edges[15][16] = 26; G->edges[15][21] = 30;
    G->edges[16][17] = 22; G->edges[16][20] = 17;
    G->edges[17][18] = 23; G->edges[17][20] = 28; G->edges[17][22] = 20;
    G->edges[18][19] = 29; G->edges[18][22] = 20; G->edges[18][24] = 31; G->edges[18][25] = 43;
    G->edges[19][25] = 29;
    G->edges[20][21] = 17; G->edges[20][23] = 30;
    G->edges[21][22] = 55; G->edges[21][23] = 41;
    G->edges[22][23] = 23; G->edges[22][24] = 23; G->edges[22][25] = 44;
    G->edges[23][24] = 34;
    G->edges[24][25] = 20;
    
    for(int i = 0; i < VERTEX_NUM; i++){
        for(int j = 0; j < VERTEX_NUM; j++){
            if(i == j)
                G->edges[i][j] = 0;
            G->edges[j][i] = G->edges[i][j];
        }
    }
    printf("Please input the startnode - 1 !!!:");
    scanf("%d", &startnode);
    printf("Please input the endnode - 1 !!!:");
    scanf("%d", &endnode);
    //while(1){
        printf("Please input the number of N:");
        scanf("%d", &N);
        printf("Please input the number of K:");
        scanf("%d", &K);
        // 全排列
        int k[K];
        if(K){
            printf("Please input the vertexs of K(attention: - 1 !!!):");
            for(int i = 0; i < K; i++)
                scanf("%d", k+i);
            Permutations(k,0,K);
        }
        int Paths[N][VERTEX_NUM];
        int Npaths[VERTEX_NUM]; 
        int Dist[N];
        for(int i = 0; i < N; i++){
            Dist[i] = 1000;
        }
        FindKpaths(G, Paths[0], Dist, Npaths);
        if(N > 1)
            FindNpaths(G, Paths, Dist, Npaths);
        for(int j = 0; j < N; j++){
            printf("The paths: ");
            for(int i = 0; i < Npaths[j]; i++)
                printf("%d ", Paths[j][i]);
            printf("The length: ");
            printf("%d\n", Dist[j]);
        }
        /*
        printf("You need input more? (0 to stop, 1 to go on)");
        int z = 1;
        scanf("%d", &z);
        if(z == 0)
            break;

        for(int j = 0; j < N; j++){
            for(int i = 0; i < Npaths[j]; i++){
                Paths[j][i] = -1;
            }
            Dist[j] = 0;
            Npaths[j] = 0;
        }
        permN = 0;
    }
    */
    return 0;
}
