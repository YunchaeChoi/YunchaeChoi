#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char **argv)
{
	int *t;
	int n=(int)strtol(argv[1], &t, 10);
	if(n<=0)	{printf("negative value\n");exit(1);}
	pid_t pid;
	int status;
	char *ptr;
	const char *shm="Collatz";
	const int SIZE = 4096;
	char msg_temp[10];
	int fd=shm_open(shm,O_RDWR | O_CREAT,0666);
	if(fd==-1)
	{
		printf("shared memory failed!\n");
		exit(1);
	}
	ftruncate(fd,SIZE); // set the size to 4,096 bytes. 
	if((pid=fork())<0)
	{
		printf("fork failed!\n");
		exit(1);
	}
	if(pid==0)
	{
		ptr=(char *)mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		sprintf(msg_temp,"%d",n);
		sprintf(ptr,"%s",msg_temp);
		ptr+=strlen(msg_temp);
		while(n>1)
		{
			if(n%2==0)
				n/=2;
			else
				n=n*3+1;
			sprintf(msg_temp,"%d",n);
			sprintf(ptr,"%s",msg_temp);
			ptr+=strlen(msg_temp);
		}
	
	}
	if(pid>0)
	{
		wait(&status);
		printf("%s \n",(char*)ptr);
		shm_unlink(shm);
		return 0;
	}


	
}

