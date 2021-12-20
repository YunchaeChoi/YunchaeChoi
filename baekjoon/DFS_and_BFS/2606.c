#include <stdio.h>
#include <stdlib.h>

int cnt=0;


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

void DFS(int** arr,int* visited, int V,int N)
{
    int i=0;
    visited[V-1]=1;
    cnt++;

    for(int i=0;i<N;i++)
    {
        if(arr[V-1][i]==1 && visited[i]==0)
        {
            DFS(arr,visited,i+1,N);
        }
    }
}

int main()
{
    int i;
    int num_computer;
    int connected;
    int e1,e2; //edge 1,2

    scanf("%d %d",&num_computer,&connected);

    int**arr = make_matrix(num_computer);
    int* visited = (int*)malloc(sizeof(int)*num_computer);
    for(i=0;i<num_computer;i++) visited[i]=0;

    init_matrix(arr,num_computer);

    for(i=0;i<connected;i++)
    {
        scanf("%d %d",&e1,&e2);
        arr[e1-1][e2-1]=1;
        arr[e2-1][e1-1]=1;
    }

   
    DFS(arr,visited,1,num_computer);
    printf("%d\n",--cnt);

    free_matrix(arr,num_computer);
    free(visited);
}