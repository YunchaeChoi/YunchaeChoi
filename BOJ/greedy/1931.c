#include <stdio.h>
#include <stdlib.h>

int* find_min(int* arr, int n) // n -> size of the array
{
    int i,min;
    int* result;
    min = arr[0];
    result = &arr[0];

    for(i=0;i<n;i++)
    {
        if(min > arr[i])
        {
            min = arr[i];
            result = &arr[i];
        }
    }
    return result;
}


int main()
{
    int n;
    scanf("%d",&n);
    int *start = (int*)malloc(sizeof(int)*n);
    int *end = (int*)malloc(sizeof(int)*n);
    int *length = (int*)malloc(sizeof(int)*n); // lengths of each conference

    int i;
    for(i=0;i<n;i++)
    {
        scanf("%d %d",&start[i],&end[i]);
        length[i] = end[i]-start[i];
    }

    int* min = find_min(length,n);





    free(start); free(end); free(length);
}
