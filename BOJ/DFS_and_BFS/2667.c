#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int i,j;
    int N;
    char line_house[8];
    scanf("%d",&N);
    int** houses = make_matrix(N);

    for(i=0;i<N;i++)
    {
        scanf("%s",line_house);
        for(j=0;j<N;j++)
        {
            houses[i][j]=line_house[j]-'0';
        }
        memset(line_house,0,sizeof(line_house));
    }

    int** arr = make_matrix(N);
    init_matrix(arr,N);

    



    free_matrix(houses,N);
    return 0;
}