#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *fib;
int n;


void* fibo(void *param);

int main(int argc, char ** argv)
{
	n= atoi(argv[1]);
	pthread_attr_t attr;
	pthread_t tid;
	fib=(int*)malloc(sizeof(int)*n);

	pthread_attr_init(&attr);
	pthread_create(&tid,&attr, fibo, NULL);
	pthread_join(tid,NULL);
	for(int i=0;i<n;i++)	printf("%d ",fib[i]);
	printf("\n");
}

void* fibo(void *param)
{
	fib[0]=0; fib[1]=1;
	for(int i=2;i<n;i++)
	{
		fib[i] = fib[i-1]+fib[i-2];
	}
	pthread_exit(0);
}
