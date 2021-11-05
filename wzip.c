#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 255

typedef struct _value
{
	int num;
	char alpha;
}value;

int main(int argc, char** argv)
{
	FILE* fp;
		
	char input[SIZE];
	char buffer[SIZE];
	char* line = NULL;
	size_t len=0;
	ssize_t nread;
	value arr[SIZE];
	int f=0; // for arr
	for(int t=0;t<argc-1;t++)
	{
		fp=fopen(argv[t+1],"r");
		nread = getline(&line,&len,fp);
		if(nread==-1)
		{
			printf("getline fail\n");
			exit(1);
		}
		fclose(fp);
		strcpy(input,line);
		free(line);
		int cnt=0;
		int j=0;
		memset(buffer,0,sizeof(buffer));
		int temp=0;
		while(1)
		{
			if(j==strlen(input)-1)
			{
				if(input[j]==buffer[cnt-1])
				{
					buffer[cnt] = input[j];
					cnt++; j++; temp++;
				}
				//fwrite(&temp,sizeof(int),1,stdout);
				//fwrite(buffer,sizeof(char),1,stdout);
				arr[f].num = temp;
				arr[f].alpha = buffer[0];
				f++;
				cnt=0; j=0; temp=0;
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
					//fwrite(&temp,sizeof(int),1,stdout);
					//fwrite(buffer,sizeof(char),1,stdout);
					arr[f].num = temp;
					arr[f].alpha = buffer[0];
					f++;
					memset(buffer,0,sizeof(buffer));
					temp=0;
				}
			}
		} // end of while

	}//end of for
	int sum=0;
	/*
	for(int i=0;i<f;i++)
	{
		if( i>0 && arr[i].alpha == arr[i-1].alpha )
		{
			while(arr[i].alpha == arr[i-1].alpha)
			{
				sum += arr[i].num + arr[i-1].num;
				i++;
				if( i+1 == f)
				{
					fwrite(&sum,sizeof(int),1,stdout);
					fwrite(&arr[i],alpha,sizeof(char(,1,stdout);
					break;
				}
			}
			fwrite( &sum, sizeof(int),1,stdout);
			fwrite(&(arr[i].alpha) ,sizeof(char) , 1, stdout);
		}
		else
		{
			fwrite(&(arr[i].num) , sizeof(int), 1, stdout);
			fwrite(&(arr[i].alpha) , sizeof(char), 1, stdout);
		}
	}
	*/
	for(int i=0;i<f;i++)
	{
		fwrite(&arr[i].num,sizeof(int),1,stdout);
		fwrite(&arr[i].alpha,sizeof(char),1,stdout);
	}
	return 0;
}
