#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>

char error_message[30] = "An error has occurred\n"; // one and only error message

char *paths[256] = {"/bin",NULL};

void print_err() // print error message
{
	write(STDERR_FILENO, error_message, strlen(error_message));
}

/*
void make_token(char* line,char* arg[10], int* i)
{
	char* token;
	assert(line!=NULL);
	token = strtok(line, " ");
	i=0;
	while(token)
	{
		arg[*i]=token;
		token=strtok(NULL, " ");
		i++;
	}
	j=i-1;
	arg[j][strlen(arg[j])-1]='\0';
	arg[*i]=NULL;

}
*/

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


		char* line = NULL;
		int status; // for wait
		size_t linecap;
		ssize_t nread;
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
				ssize_t a=0;
				paths[0]=NULL;
				for(;a<i-1;a++)
					paths[a] = strdup(arg[a+1]);
				paths[a+1] = NULL;
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
			}
			if(pid==0)
			{
				//printf("child: my pid is... %d\n",getpid());
				char path[256];
				int paths_idx=0;
				while(paths[paths_idx] !=NULL)
				{
					sprintf(path,"%s/%s", paths[paths_idx++], arg[0]);
					if(access(path,X_OK)==0) break;
				}
				if(access(path,X_OK)==-1)
				{
					print_err(); // binary access fail
					_exit(status);
				}
				int check_redirect=0;
				int is_redirect=0; // is_redirect==1 means redirect.
				for(;check_redirect<i;check_redirect++)
				{
					if(strstr(arg[check_redirect],">")!=NULL)
					{
						is_redirect=1;
						break;
					}
				}
				if(is_redirect) // redirect to some file, or buffer. Not stdout.
				{
					if(strcmp(arg[check_redirect],">")==0) // if arg[check_redirect] is only ">", not something like "tests>/tmp/output".
					{
						if(check_redirect+1==i) // redirect, bot no output file
						{
							print_err();
							_exit(status);
						}
						int fd = open(arg[++check_redirect], O_WRONLY | O_CREAT, 0666);
						dup2(fd,1); // make stdout go to file
						//dup2(fd,2); // make stderr go to file (maybe I don't have to do this?)
						close(fd);
						
						if(check_redirect+1<i) // redirect, but two output. Or, two redirection
						{
							print_err();
							_exit(status);
						}

					}
					else // arg[check_redirect] is something like "tests/p2a-test>/tmp/output.11" or "tests/p2a-test>" or "ls>output"
					{
						//-------------make token-------------//

						char* token_rd; // token for redirect
						char* arg_rd[10];
						assert(arg[check_redirect]!=NULL);
						token_rd = strtok(arg[check_redirect], " ");
						int i_rd=0;
						while(token_rd)
						{
							arg_rd[i]=token_rd;
							token_rd=strtok(NULL, " ");
							i_rd++;
						}
						int j_rd=i_rd-1;
						arg_rd[j_rd][strlen(arg[j])-1]='\0';
						arg_rd[i_rd]=NULL;

						//-------------make token----------//
						


					}

					if(check_redirect+1==i) // redirect, but no output file defined.
					{
						print_err();
						_exit(status);
					}

					if(check_redirect+1 < i ) // redirect, but two output
					{
						print_err();
						_exit(status);
					}
					arg[--check_redirect]=NULL;

				}
				execv(path,arg);
				printf("This shouldn't be printed\n");
			}
			if(pid>0)
			{
				//printf("parent: I'm parent of... %\n", pid);
				wait(&status);
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
			

			pid_t pid = fork();
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
				if(flag==0)	exit(0);
				//free(line);
			}
			
			
		}
	}
	return 0;
}
