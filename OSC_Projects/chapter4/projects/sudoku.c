#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
	int row;
	int column;
}parameters;

int arr[9][9];

void *check_column(int* arr[9]);

void *check_row(int* arr[9]);

void *check_subgrids(int* arr[9]);

int main()
{
	parameters *data = (parameters *)malloc(sizeof(parameters));
	data->row = 1;
	data->column =1;

	
}

void *check_column(int* arr[9], int number)
{
    int c_idx=number; //column index
    for(int i=0;i<9;i++)
    {
        
    }


}

void *check_row()

void *check_subgrids()


