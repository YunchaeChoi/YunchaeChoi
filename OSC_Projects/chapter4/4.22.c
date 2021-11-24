// a multithreaded program that caluculates various statistical values for a list of numbers.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int av=0;
int min=0;
int max=0;
int arr[7];

void *average()
{
	int i=0;
	int sum=0;
	for(;i<sizeof(arr)/sizeof(int);i++)
		sum+=arr[i];
	av=sum/(sizeof(arr)/sizeof(int));
}
void *minimum()
{
	int temp=arr[0];
	for(int i=1;i<sizeof(arr)/sizeof(int);i++)
	{
		if(temp>arr[i])
			temp = arr[i];
	}
	min = temp;
}
void *maximum()
{
	int temp=arr[0];
	for(int i=1;i<sizeof(arr)/sizeof(int);i++)
	{
		if(temp<arr[i])
			temp = arr[i];
	}
	max = temp;
}


int main()
{
	int i=0;
	for(;i<7;i++)
		scanf("%d",&arr[i]);

	pthread_t tid[3];

	pthread_create(&tid[0], NULL, average,arr);
	pthread_join(tid[0],NULL);

	pthread_create(&tid[1], NULL, minimum,arr);
	pthread_join(tid[1],NULL);

	pthread_create(&tid[2], NULL, maximum,arr);
	pthread_join(tid[2],NULL);

	printf("The average : %d\n",av);
	printf("The minimum : %d\n",min);
	printf("The maximum : %d\n",max);
}
