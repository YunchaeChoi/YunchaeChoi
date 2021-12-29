#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int** matrix;
int* vector;
int* new_matrix;
int n,m;

void show_matrix()
{
    printf("*** matrix ***\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

void show_vector()
{
    printf("*** vector ***\n");
    for(int i=0;i<m;i++)
        printf("%d\n",vector[i]);
}

void *mul(void *data)
{
    int i = (int)data;
    for(int column=0;column<m;column++)
    {
        new_matrix[i] += matrix[i][column]*vector[column];
    }
}

void show_new_matrix()
{
    printf("*** new matrix ***\n");
    for(int i=0;i<n;i++)
        printf("%d\n",new_matrix[i]);
}



int main(int argc, char* argv[])
{
    int i,j;
    srand(time(NULL));
    n = argv[1][0]-'0';
    m = argv[2][0] -'0';

    matrix = (int**)malloc(sizeof(int*)*n);
    for(i=0;i<n;i++)
    {
        matrix[i]=(int*)malloc(sizeof(int)*m);
    }
    vector = (int*)malloc(sizeof(int)*m);

    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            matrix[i][j]=(rand()%10);
        }
    }
    for(i=0;i<m;i++)
        vector[i]=(rand()%10);
    new_matrix=(int*)malloc(sizeof(int)*n);
    for(i=0;i<n;i++)    new_matrix[i]=0;

    show_matrix();
    show_vector();


    pthread_t tid[m]; // threads
    long t;

    for(t=0;t<(long)n;t++)
    {
        if(pthread_create(&tid[(int)t],NULL,mul,(void*)t))
        {
            printf("pthread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    for(t=0;t<(long)n;t++)
    {
        pthread_join(tid[(int)t],NULL);
    }
    printf("\n\n");
    show_new_matrix();


    for(i=0;i<n;i++)
        free(matrix[i]);
    free(matrix);
    free(vector);
    free(new_matrix);

}
