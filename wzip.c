#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 255

int main(int argc, char** argv)
{
	FILE* fp;
	//FILE* fw; // file to write on, i.e., file.z
	//fw = fopen("file.z","a+");

	//int size;
	
	char input[SIZE];
	char buffer[SIZE];
	//char path[SIZE];
	//char *output_c; // char
	//int *output_i; // int
	//char output[SIZE];
	//memset(output,0,sizeof(output));
	char* line = NULL;
	size_t len=0;
	ssize_t nread;

	fp=fopen(argv[argc-1],"r");
	nread = getline(&line,&len,fp);
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
	int temp;
	while(j<strlen(input)) //  j < 13
	{
		if(j==strlen(input)-1)
		{
			//printf("%d%c",(int)strlen(buffer),buffer[0]);
			//temp =(int)strlen(buffer);
			fwrite(&temp,sizeof(int),1,stdout);
			fwrite(buffer,sizeof(char),1,stdout);
			break;
		}
		if(cnt==0)
		{
			buffer[cnt] = input[j];
			cnt++; j++; temp++;
		}
		else
		{
			if(input[j]==buffer[cnt-1])
			{
				buffer[cnt] = input[j];
				cnt++; j++; temp++;
			}
			else
			{
				cnt=0;
				//printf("%d%c",(int)strlen(buffer),buffer[0]);
				//temp=(int)strlen(buffer);
				fwrite(&temp,sizeof(int),1,stdout);
				fwrite(buffer,sizeof(char),1,stdout);
				memset(buffer,0,sizeof(buffer));
				temp=0;
			}
		}
	}
	return 0;
}
