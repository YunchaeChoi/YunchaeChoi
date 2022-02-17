#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define VALID 1
#define INVALID 0

typedef struct _parameters
{
	int row;
	int column;
}parameters;

int sudoku[9][9]; // sudoku puzzle.

void print_array(int* arr)
{
	printf("printing arrays\n");
	for(int i=0;i<9;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int one_to_nine_column(int sudoku[9][9],int column)
{
	int number[9] = {0,};
	for(int i=0;i<9;i++)
	{
		number[sudoku[column-1][i]-1]=1;
	}
	for(int i=0;i<9;i++)
	{
		if(number[i]==0)
		{
			printf("why INVALID?: %d - %d th column\n",i,column);
			return INVALID; // number missing
		}
	}
	return VALID; // valid

}

int one_to_nine_row(int sudoku[9][9],int row)
{
	int number[9] = {0,};
	for(int i=0;i<9;i++)
	{
		number[sudoku[i][row-1]-1]=1;
	}
	for(int i=0;i<9;i++)
	{
		if(number[i]==0)
		{
			printf("why INVALID?: %d is missing - %dth row\n",i+1,row);
			return INVALID; // number missing
		}
	}
	return VALID; // valid

}

int one_to_nine_subgrid(parameters* data)
{
	int number[9] = {0,};
	for(int i=data->column;i< data->column+3 ;i++)
	{
		for(int j=data->row;j< data->row+3 ;j++)
		{
			number[sudoku[i][j]-1]=1;
		}
	}
	for(int i=0;i<9;i++)
	{
		if(number[i]==0)
		{
			return INVALID; // number missing
		}
	}
	return VALID;
}

void *check_column(void* col)
{
	int number = *((int *)col);
	if(one_to_nine_column(sudoku,number)==VALID)
	{
		return (void *)VALID;
	}
	else
	{
		return (void *)INVALID;
	}
}

void *check_row(void* row)
{
	int number=*((int *)row);
	if(one_to_nine_row(sudoku,number) == VALID)
	{
		return (void* )VALID;
	}
	else
		return (void *)INVALID;
}

void init_subgrid(int subgrid[3][3])
{
		for(int i=0;i<3;i++)
		{
				for(int j=0;j<3;j++)
				{
						subgrid[i][j]=0;
				}
		}
}

void *check_subgrid(void* data_argument) // parameters를 건네받음.
{
	parameters *data ;
	memcpy(&data,&data_argument,sizeof(parameters));

	if(one_to_nine_subgrid(data)==VALID)
	{
		return (void *)VALID;
	}
	else
		return (void *)INVALID;
}

int main(int argc, char* argv[])
{
	FILE* fp = fopen(argv[1],"r");
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			fscanf(fp, "%d", &sudoku[i][j]);
			printf("%d ",sudoku[i][j]);
		}
		printf("\n");
	}

	parameters *data = (parameters *)malloc(sizeof(parameters));
	data->row = 0;
	data->column =0;


	pthread_t tid[27]; // 9 threads to check 3*3 subgrids + 9 threads to check a column + 9 threads to check a row  

	int column_thread=0;
	int row_thread=0;
	int subgrid_thread=0;
	printf("creation starts.. now\n");
	for(column_thread=0;column_thread<9;column_thread ++)
	{
		pthread_create(&tid[column_thread],NULL,check_column,(void *)&data->column);
		printf("creating columns: %d\n",data->column);
		data->column++;
	}

	for(row_thread=9;row_thread<18;row_thread ++)
	{
		pthread_create(&tid[row_thread],NULL,check_row,(void *)&data->row);
		printf("creating row: %d\n",data->row);
		data->row++;
	}


	data->row=0; data->column=0;

	for(subgrid_thread=18;subgrid_thread<27;subgrid_thread++)
	{
		if(data->row < 9 && data->column <9)
		{
			if((pthread_create(&tid[subgrid_thread],NULL,check_subgrid,(void *)data)) !=0  )
				printf("subgrid thread not created\n");
			printf("created subgrid: %d %d\n",data->column,data->row);
		}

		data->column+=3;
		if(data->column == 9)
		{
			data->column=0;
			data->row+=3;
		}
	}

	int results[27]; // the sudoku is valid only if this array is all 1. ( VALID )

	for(int i=0;i<27;i++)
	{
			results[i]=-1;
	}
	printf("\n");

	int valid_bit=VALID;
	for(int i=0;i<27; i++)
	{
		if((pthread_join(tid[i],(void * )&results[i])) != 0)
		{
			printf("thread not joined. tid: %d\n",i);
		}
		printf("joined. result: %d -- %d\n",results[i],i);
		if(results[i]==0)
		{
			valid_bit=INVALID;
			break;
		}
	}
	if(valid_bit)
	{
		printf("This sudoku is valid\n");
	}
	else
	{
		printf("This sudoku is invalid\n");
	}
	free(data);
}
