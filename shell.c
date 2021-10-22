#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */

int main(int argc, char** argv)
{
	char path[MAX_LEN];
	char* cmd=NULL;
	pid_t pid;
	int status;
	size_t size;
	char *pwd;

	int should_run = 1; /* flag determines when to exit program */

	while(1)
	{
		sprintf(pwd,"%s",getenv("PWD"));
		printf("choi@DESKTOP: %s/ycs> ",pwd);
		fflush(stdout);
		getline(&cmd, &size, stdin);
		cmd[strlen(cmd) -1] = '\0';
		// int fd[2];
		char *ptr = strtok(cmd," ");
		char *arg[100];
		int i=0;
		while(ptr!=NULL)
		{
			arg[i++]=ptr;
			ptr = strtok(NULL," ");
		}
		arg[i]=NULL;
		if(strcmp(arg[0],"exit")==0)
		{
			should_run=0;
			break;
		}
		sprintf(path,"/bin/%s",arg[0]);
		if((pid=fork())<0)
		{
			printf("fork fail\n");
			return 1;
		}
		if(pid==0)
		{
			if(strcmp(arg[0],"history")==0) /* command history */
			{
				execl("/bin/cat","cat","/home/hi/.bash_history",NULL);
			}
			execvp(path, arg);
		}
		if(pid>0)
		{
			wait(&status);
		}


	}
	return 0;
}
