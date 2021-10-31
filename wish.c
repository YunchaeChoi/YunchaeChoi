#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	while(1)
	{
		char* line = NULL;
		int status; // for wait
		size_t linecap;
		char path[30];
		printf("wish> ");
		getline(&line,&linecap,stdin);
		pid_t pid = fork();
		if(pid<0)
		{
			printf("fork failed!\n");
			exit(1);
		}
		if(pid==0)
		{
			char** ap, *arg[10];
			assert(line!=NULL);
			for(ap=arg; (*ap=strsep(&line," ")) != NULL;)
					if(**ap != '\0')
						if(++ap >= &arg[10])
							break;
			*ap=NULL;
			sprintf(path,"/bin/%s",arg[0]);
			if(access(path,AT_EACCESS)==-1)
			{
				sprintf(path,"/usr/bin/%s",arg[0]);
			}
			
			execv(path,arg);
		}
		if(pid>0)
		{
			wait(&status);
			free(line);
		}
		
		
	}
	return 0;
}
