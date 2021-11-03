#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 255

int main(int argc, char** argv)
{
	FILE* fp;
	FILE* fw; // file to write on, i.e., file.z
	//int size;
	
	char input[SIZE];
	char buffer[SIZE];
	//char path[SIZE];
	char *output_c; // char
	int *output_i; // int

	//memset(output,0,sizeof(output));
	char* line = NULL;
	size_t len=0;
	ssize_t nread;

	fp=fopen(argv[argc-1],"r");
	nread=getline(&line, &len, fp);
	if(nread==-1)
	{
		printf("getline fail\n");
		exit(1);
	}
	strcpy(input,line);
	free(line);
	int cnt=0;
	int j=0;
	//char *c;
	memset(buffer,0,sizeof(buffer));
	//printf("%ld\n",strlen(input));
	
	while(j<strlen(input)-1) //  j < 13
	{
		if(j==strlen(input)-2)
		{
			printf("%d%c",(int)strlen(buffer),buffer[0]);
			break;
		}
		if(cnt==0)
		{
			buffer[cnt] = input[j];
			cnt++; j++;
			//printf("%d\n",cnt);
		}
		else
		{
			if(input[j]==buffer[cnt-1])
			{
				buffer[cnt] = input[j];
				cnt++; j++;
				//printf("%d\n",cnt);
			}
			else
			{
				cnt=0;
				printf("%d%c",(int)strlen(buffer),buffer[0]);
				memset(buffer,0,sizeof(buffer));
			}
		}
	}
	return 0;
}
