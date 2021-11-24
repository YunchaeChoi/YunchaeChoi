#include <stdio.h>
#include <stdlib.h>
#include <math.h> // compile with -lm

int offset[12];
int page[20];

void dec_to_bin(int* a)
{
	int i=0;	
	for(;i<12;i++)
	{
		offset[11-i]=*a%2;
		*a/=2;
	}
	for(i=0;i<20;i++)
	{
		page[19-i]=*a%2;
		*a/=2;
	}
}

int bin_to_dec(int *arr,int size)
{
	int result=0;
	for(int i=0;i<size;i++)
	{
		result += arr[size-1-i]*(int)pow(2.0,i);
	}
	return result;
}

int main(int argc,char* argv[])
{
	char *endptr;
	int a = (int)strtol(argv[1],&endptr,10);
	printf("The address %d contains:\n",a);
	dec_to_bin(&a);
	printf("page number = ") ;
	printf("%d\n",bin_to_dec(page,20));
	printf("offset = ");
	printf("%d\n",bin_to_dec(offset,12));
	return 0;
}
