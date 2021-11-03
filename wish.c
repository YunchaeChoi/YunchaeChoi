#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int i=0;
	while(1)
	{
		int flag=1;
		char* line = NULL;
		int status; // for wait
		size_t linecap;
		char path[30];

		printf("wish> ");
		getline(&line,&linecap,stdin);
		
		int fd[2];
		pipe(fd);

		pid_t pid = fork();
		if(pid<0)
		{
			printf("fork failed!\n");
			exit(1);
		}
		if(pid==0)
		{
			char* token;
			char* arg[10];
			assert(line!=NULL);
			token = strtok(line, " ");
			i=0;
			while(token!=NULL)
			{
				arg[i]=token;
				token=strtok(NULL, " ");
				i++;
			}
			int j=i-1;
			arg[j][strlen(arg[j])-1]='\0';
			arg[i]=NULL;
			if(strcmp(arg[0],"cd")==0)
			{
				if(i==1)
				{
					if(chdir("/home/hi")==-1)
					{
						printf("chdir error\n");
						exit(1);
					}
				}
				else
				{
					if(strcmp(arg[1],"~")==0)
					{
						if(chdir("/home/hi")==-1)
						{
							printf("chdir error\n");
							exit(1);
						}
					}
					else
					{
						if(chdir(arg[1])==-1)
						{
							printf("chdir error\n");
							exit(1);
						}
					}
				}
				continue;	
			}
			/*
			if(strcmp(arg[0],"path")==0)
			{
					
			}
			*/
			if(strcmp(arg[0],"exit")==0)
			{
				flag=0;
				close(fd[0]);
				write(fd[1],&flag,sizeof(flag));
				close(fd[1]);
				exit(0);
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
			close(fd[1]);
			read(fd[0],&flag,sizeof(flag));
			close(fd[0]);
			if(flag==0)	exit(0);
			free(line);
		}
		
		
	}
	return 0;
}
