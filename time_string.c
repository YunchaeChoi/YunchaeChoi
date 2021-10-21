#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char ** argv)
{
	pid_t pid;
	int fd[2];
	int status;
	char* ptr;
	char start_time[255];
	char end_time[255];
	char path[20];
	struct timeval start,end;
	sprintf(path,"/bin/%s", argv[1]);
	if( pipe(fd) == -1)
		printf("pipe fail\n");
	if((pid=fork())==0) // child
	{
		close(fd[0]);
		gettimeofday(&start,NULL);
		sprintf(start_time, "%ld", start.tv_usec);
		int r = write(fd[1],start_time,strlen(start_time)+1);
		printf("write : %d\n", r ) ;
		execl(path, argv[1],argv[2],NULL);
		close(fd[1]);
	}
	if(pid>0) // parent
	{
		wait(NULL);
		close(fd[1]);
		read(fd[0], start_time, 255);
		gettimeofday(&end,NULL);
	
		long int start = strtol(start_time, &ptr, 10);
		long int diff = end.tv_usec - start;
		printf("\nElapsed time: %f\n", (double)diff/1000000 ) ;
		close(fd[0]);
		return 0;
	}
}
