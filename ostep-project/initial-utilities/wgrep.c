#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 15000

int main(int argc, char** argv)
{
	char buffer[SIZE];
	char* token;
	const char s[2] = " ";
	size_t len =0;
	ssize_t read;

	if(argc==1)
	{
		printf("wgrep: searchterm [file ...]\n");
		exit(1);
	}
	if(argc==2)
	{
		char buffer_origin[SIZE];
		int flag=0; // if flag is 0, don't printf that line
		while(1)
		{
			if(fgets(buffer,SIZE,stdin)==NULL)	break;
			strcpy(buffer_origin,buffer);
			token = strtok(buffer,s);
			while(token!=NULL)
			{
				if(strstr(token,argv[1])!=NULL)
				{
					flag=1;
					printf("%s",buffer_origin);
					break;
				}
				token=strtok(NULL,s);
			}
			buffer[0]='\0';
		}
		return 0;
	}
	for(int j=0;j<argc-2;j++)
	{
		int flag=0;
		char line_origin[SIZE];
		char* line =NULL;
		FILE *fp = fopen(argv[j+2],"r");
		if(fp==NULL)
		{
			printf("wgrep: cannot open file\n");
			exit(1);
		}
		while((read=getline(&line,&len,fp)) != -1)
		{
			strcpy(line_origin,line);
			token = strtok(line,s);
			while(token!=NULL)
			{
				flag=0;
				if(strstr(token,argv[1])!=NULL)
				{
					printf("%s",line_origin);
					flag=1;
					break;
				}
				token = strtok(NULL,s);
			}
			
		}
		free(line);
	}
	return 0;
}

