#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int i;
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
			char* token, *arg[10];
			assert(line!=NULL);
			token = strtok(line, " ");
			i=0;
			while(token != NULL)
			{
				arg[i] = token;
				token = strtok(NULL," ");
				i++;
			}
			arg[--i][strlen(arg[i])-1]='\0';
			arg[++i]=NULL;
			if(strcmp(arg[0],"exit")==0)
			{
				printf("executing exit\n");
				exit(0);
			}
			if(strcmp(arg[0],"cd")==0)
			{
				if(chdir(arg[1])==-1)
				{
					printf("chdir error\n");
					exit(1);
				}	
			}
			if(strcmp(arg[0],"path")==0)
			{
			}
			sprintf(path,"/bin/%s",arg[0]);
			if(access(path,X_OK)==-1)
			{
				sprintf(path,"/usr/bin/%s",arg[0]);
				if(access(path,X_OK)==-1)
				{
					printf("binary access fail\n");
				}
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
