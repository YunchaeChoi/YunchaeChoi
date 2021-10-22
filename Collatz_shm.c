#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

int main(int argc, char **argv)
{
	char *t;
	int n=(int)strtol(argv[1], &t, 10);
	if(n<=0)	{printf("negative value\n");exit(1);}
	pid_t pid;
	int status;
	char *ptr;
	const char *shm="Collatz";
	const int SIZE = 4096;
	char msg_temp[128];
	char *blank = " ";
	int fd=shm_open(shm,O_RDWR | O_CREAT,0666);
	if(fd==-1)
	{
		printf("shared memory failed!\n");
		return 1;
	}
	ftruncate(fd,SIZE); // set the size to 4096 bytes. 
	if((pid=fork())<0)
	{
		printf("fork failed!\n");
		return 1;
	}
	if(pid==0)
	{
		ptr=mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if(ptr ==MAP_FAILED)
		{
			printf("Map failed!\n");
			exit(-1);
		}
		sprintf(msg_temp,"%d",n);
		sprintf(ptr,"%s",msg_temp);
		
		while(n!=1)
		{
			ptr+=strlen(msg_temp);
			sprintf(ptr,"%s",blank);
			ptr+=strlen(blank);
			
			if(n%2==0)
				n/=2;
			else
				n=n*3+1;
			sprintf(msg_temp,"%d",n);
			sprintf(ptr,"%s",msg_temp);
			
		}
	
	}
	if(pid>0)
	{
		wait(&status);
		ptr=(char *)mmap(0,SIZE,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		while(ptr!=NULL)
		{
			if(*ptr-'0'<0 || *ptr-'0'>9 )
			{
				if(*ptr== ' ')
				{
					printf(" ");
					ptr+=1;
					continue;
				}
				else
					break;
			}

			printf("%c",*ptr);
			ptr+=1;
		}
		printf("\n");
		shm_unlink(shm);
		return 0;
	}


	
}

