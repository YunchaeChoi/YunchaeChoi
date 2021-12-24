#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 50

int **maze;
int *visited;

typedef struct _GraphNode
{
    int vertex;
    struct _GraphNode* link;
}GraphNode;

typedef struct _GraphType
{
    int n; // number of vertices of the graph
    GraphNode* adj_list[MAX_VERTICES];
    // 그래프 정점에 대한 헤드 노드
}GraphType;

void init(GraphType* g)
{
    int v;
    g->n=0;
    // initializes the number of vertices of the graph to zero.
    for(v=0;v<MAX_VERTICES;v++)
        g->adj_list[v]=NULL;
    // initialized the array of the head nodes of the graph to NULL
}

void insert_vertex(GraphType* g, int v)
{
    if( (g->n)+1  > MAX_VERTICES)
    {
       printf("graph: 정점의 개수 초과\n");
       return;
    }

    g->n++;
}

void insert_edge(GraphType *g,int u, int v)
{
    GraphNode *node;
    if(u >=g->n || v>=g->n)
    {
        printf("graph: 정점 번호 오류\n");
        return;
    }
    node = (GraphNode*)malloc(sizeof(GraphNode));
    node->vertex=v;
    node->link = g->adj_list[u];
    g->adj_list[u]=node;
}

void print_adj_list(GraphType * g) {
	for (int i = 0; i < g->n; i++) {
		GraphNode* p = g->adj_list[i];
		printf("정점 %d의 인접 리스트", i);
		while (p != NULL) {
			printf("-> %d", p->vertex);
			p = p->link;
		}
		printf("\n");
	}
}

int** make_matrix(int N,int M)
{
    int ** arr = (int**)malloc(sizeof(int*)*N);
    for(int i=0;i<M;i++)
        arr[i]=(int*)malloc(sizeof(int)*M);

    return arr;
}

void init_matrix(int** arr, int N)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            arr[i][j]=0;
        }
    }
}

void free_matrix(int **arr,int N)
{
    for(int i=0;i<N;i++)
        free(arr[i]);
    free(arr);
}

void show_matrix(int** arr,int N)
{
    int i,j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }
}

void DFS(int** arr,int V,int N)
{
    int i=0;
    visited[V-1]=1;

    for(int i=0;i<N;i++)
    {
        if(arr[V-1][i]==1 && visited[i]==0)
        {
            DFS(arr,i+1,N);
        }
    }
}

/*
 * 1 2 3 4 5
 * 6 7 8 9 10 ....
 * --------------
 *  each block of maze, vertex numbers increase in this order.
*/


int main()
{
    int n,m;
    GraphType* g;
    g= (GraphType*)malloc(sizeof(GraphType));
    init(g);

    scanf("%d %d",&n,&m);
    maze=make_matrix(n,m);
    visited = (int*)malloc(sizeof(int)*n*m);
    
    char *line= (char*)malloc(sizeof(char)*(m+1));
    int i,j;
    for(i=0;i<n;i++)
    {
        scanf("%s",line);
        for(j=0;j<m;j++)
        {
            maze[i][j]=line[j]-'0';
        }
    }
    free(line);

    int vertex_num=1;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            insert_vertex(g,vertex_num);
            vertex_num++;
        }
    }



    free(g);
    free(visited);
    free_matrix(maze,n);

    return 0;
}

