#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int** make_matrix(int N)
{
    int ** arr = (int**)malloc(sizeof(int*)*N);
    for(int i=0;i<N;i++)
        arr[i]=(int*)malloc(sizeof(int)*N);

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

void free_matrix(int **arr,int N)
{
    for(int i=0;i<N;i++)
        free(arr[i]);
    free(arr);
}

void DFS(int** arr,int* visited, int V,int N)
{
    int i=0;
    visited[V-1]=1;
    printf("%d ",V);

    for(int i=0;i<N;i++)
    {
        if(arr[V-1][i]==1 && visited[i]==0)
        {
            DFS(arr,visited,i+1,N);
        }
    }
}

typedef struct Queue queue;

void BFS(int** arr,int* visited, int V,int N, queue* q)
{
    int i;

    visited[V-1]=1;
    printf("%d ",V);
    enQueue(q,V);

    while(!IsEmptyQueue(q))
    {
        V=deQueue(q);
        for(i=0;i<N;i++)
        {
            if(arr[V-1][i]==1 && visited[i]==0)
            {
                printf("%d ",i+1);
                visited[i]=1;
                enQueue(q,i+1);
            }
        }
    }
}



int main()
{
    int N,M,V;
    int i=0, j=0;
    int e1, e2; // edge 1,2
    scanf("%d %d %d",&N,&M,&V);

    queue* q = createQueue();
    

    int** arr = make_matrix(N);
    int* visited = (int*)malloc(sizeof(int)*N);
    for(i=0;i<N;i++) visited[i]=0;

    init_matrix(arr,N);

    for(i=0;i<M;i++)
    {
        scanf("%d %d",&e1,&e2);
        arr[e1-1][e2-1]=1;
        arr[e2-1][e1-1]=1;
    }
   // printf("--------------------\n");

    DFS(arr,visited,V,N);
    printf("\n");
    for(i=0;i<N;i++) visited[i]=0;
    BFS(arr,visited,V,N,q);
    printf("\n");
    
    free_matrix(arr,N);

    return 0;
}