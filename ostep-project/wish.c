#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

char error_message[30] = "An error has occurred\n"; // one and only error message

void print_err() // print error message
{
	write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char** argv)
{

	if(argc>2) // more than one file
	{
		print_err();
		exit(1);
	}

	int i=0;
	int flag;

	if(argc>1)
	{

		FILE *fp;
		fp=fopen(argv[1],"r");

		if(fp==NULL) // cannot open file
		{
			print_err();
			exit(1);
		}

		setenv("PATH", "/bin",1);

		char* line = NULL;
		int status; // for wait
		size_t linecap;
		ssize_t nread;
		char path[30];
		while(1)
		{
			if((nread=getline(&line,&linecap,fp))==-1) // EOF. means that the last line of argv[1] is not "exit"
			{
				exit(0);
			}
			char* token;
			char* arg[10];
			assert(line!=NULL);
			token = strtok(line, " ");
			i=0;
			while(token!=NULL)
			{
				arg[i]=token;
				token=strtok(NULL, " ");
				i++;		 // after while loop, i equals the number of components in arg.
			}            		// arg[0] = command. (e.g., "cd" )
			int j=i-1;
			arg[j][strlen(arg[j])-1]='\0';
			arg[i]=NULL;
			if(strcmp(arg[0],"cd")==0)
			{
				if(i==1)
				{
					print_err();  
				}
				if(i>2)
				{
					print_err();
				}
				else if(i==2)
				{
					if(strcmp(arg[1],"~")==0)
					{
						if(chdir(getenv("HOME"))==-1)
						{
							print_err();
						}
					}
					else
					{
						if(chdir(arg[1])==-1)
						{
							print_err(); 
						}
					}
				}
				continue;
			}
			if(strcmp(arg[0],"path")==0)
			{
				if(i==1) //command 'path' is passed with 0 argument.
				{
					setenv("PATH","",1);
				}
				else if(i>1)
				{
					char new_path[255];
					memset(new_path,0,sizeof(new_path));
					for(int y=0;y<i-1;y++)
					{
						if(y>0)
							strcat(new_path,":");
						strcat(new_path,arg[y+1]);
					}
					setenv("PATH",new_path,1);
				}
				//printf("revised path: %s\n",getenv("PATH"));
				continue;
			}
			if(strcmp(arg[0],"exit")==0)
			{
				if(i>1) // one or more arguments passed to exit, error
				{
					print_err();
					exit(0);
				}
				exit(0);	
			}


			pid_t pid = fork();
			if(pid<0)
			{
				print_err(); // fork fail
				printf("fork fail\n");
			}
			if(pid==0)
			{
				//printf("child: my pid is... %d\n",getpid());
				if(strstr(arg[0],".sh")!=NULL) // if arg[0] contains ".sh"
				{
					strcpy(path,getenv("PATH"));
					strcat(path,"/");
					strcat(path,arg[0]);
					/*
					if(access(path,X_OK)==-1)
					{
						if(access(path,X_OK)==-1)
						{
							print_err(); // binary access fail
							_exit(status);
						}
					}
					*/
					execv(path,arg);
				}
				sprintf(path,"/bin/%s",arg[0]);
				if(access(path,X_OK)==-1)
				{
					sprintf(path,"/usr/bin/%s",arg[0]);
					if(access(path,X_OK)==-1)
					{
						print_err(); // binary access fail
						_exit(status);
					}
				}
				else
					execv(path,arg);
				//printf("execv... \n");
					//printf("execv error\n");
				printf("This shouldn't be printed\n");
			}
			if(pid>0)
			{
				wait(&status);
				//printf("parent: I'm parent of... %d\n",pid);
			}

		}
		free(line);
	} // end of if(argc>1)
	
	else if(argc==1)
	{
		while(1)
		{
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
						if(chdir(getenv("HOME"))==-1)
						{
							printf("chdir error\n");
							exit(1);
						}
					}
					else
					{
						if(strcmp(arg[1],"~")==0)
						{
							if(chdir(getenv("HOME"))==-1)
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
				}
				/*
				if(strcmp(arg[0],"path")==0)
				{
						
				}
				*/
				if(strcmp(arg[0],"exit")==0)
				{
					if(i>1) // one or more arguments passed to exit, error
					{
						print_err();
						exit(1);
					}
				
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
					if(strcmp(arg[0],"cd")!=0 && access(path,X_OK)==-1)
					{
						printf("binary access fail\n");
					}
				}	
				execv(path,arg);
			}
			if(pid>0)
			{
				flag=1;
				wait(&status);
				close(fd[1]);
				read(fd[0],&flag,sizeof(flag));
				close(fd[0]);
				if(flag==0)	exit(0);
				//free(line);
			}
			
			
		}
	}
	return 0;
}
