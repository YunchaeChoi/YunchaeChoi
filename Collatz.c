#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int* ptr;
	int n=(int)strtol(argv[1], &ptr, 10);
	if(n<=0)	{printf("negative value\n");exit(1);}
	pid_t pid;
	int status;
	if((pid=fork())<0)
	{
		printf("fork failed!\n");
		exit(1);
	}
	if(pid ==0) // child
	{
		printf("%d ",n);
		while(n>1)
		{
			if(n%2==0)
			{
				n/=2;
				printf("%d ",n);
			}
			else
			{
				n=3*n+1;
				printf("%d ",n);
			}
		}
		printf("\n");
	}
	if(pid>0) // parent
	{
		wait(&status);
		printf("parent done\n");
		return 0;
	}

	
}

