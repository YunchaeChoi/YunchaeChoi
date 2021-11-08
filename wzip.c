#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 15000

typedef struct _value
{
	int num;
	char alpha;
}value;

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		printf("wzip: file1 [file2 ...]\n");
		return 1;
	}
	FILE* fp;
		
	char input[SIZE];
	char buffer[SIZE];
	size_t len=0;
	ssize_t nread;
	value arr[SIZE];
	int f=0; // for arr
	for(int t=0;t<argc-1;t++)
	{
		char * line = NULL;
		fp=fopen(argv[t+1],"r");
		while((nread=getline(&line,&len,fp)!=-1 ))
		{
			fclose(fp);
			//strcpy(input,line);
			//free(line);
			int cnt=0;
			int j=0;
			memset(buffer,0,sizeof(buffer));
			int temp=0;
			while(1)
			{
				if(line[j] == '\n')
				{
					arr[f].num = temp;
					arr[f].alpha = buffer[0];
					f++;
					arr[f].num = 1;
					arr[f].alpha = '\n';
					f ++; j=0; cnt=0; temp=0;
					break;
				}
				if(j==strlen(line)-1)
				{
					if(line[j]==buffer[cnt-1])
					{
						buffer[cnt] = line[j];
						cnt++; j++; temp++;
					}
					arr[f].num = temp;
					arr[f].alpha = buffer[0];
					f++;
					cnt=0; j=0; temp=0;
					break;
				}
				if(cnt==0)
				{
					buffer[cnt] = line[j];
					cnt++; j++; temp++;
				}
				else
				{
					if(line[j]==buffer[cnt-1])
					{
						buffer[cnt] = line[j];
						cnt++; j++; temp++;
					}
					else
					{
						cnt=0;
						arr[f].num = temp;
						arr[f].alpha = buffer[0];
						f++;
						memset(buffer,0,sizeof(buffer));
						temp=0;
					}
				}
			} // end of while
		} // end of while (nread)
		free(line);
	}//end of for
	int sum=0;
	int i=0;
	while(1)
	{
		if(sum==0)
		{
			if(i==f-1)
			{
				sum+= arr[i].num;
				fwrite(&sum,sizeof(int),1,stdout);
				fwrite(&arr[i].alpha,sizeof(char),1,stdout);
				break;
			}
			else
			{
				sum += arr[i].num;
				i++;
			}
		}
		else // if sum is not zero
		{
			if(i==f-1)
			{
				if ( arr[i].alpha == arr[i-1].alpha )
				{
					sum += arr[i].num;
					fwrite(&sum,sizeof(int),1,stdout);
					fwrite(&arr[i].alpha,sizeof(char),1,stdout);
					break;
				}
				else
				{
					fwrite(&sum,sizeof(int),1,stdout);
					fwrite(&arr[i-1].alpha,sizeof(char),1,stdout);
					fwrite(&arr[i].num,sizeof(int),1,stdout);
					fwrite(&arr[i].alpha,sizeof(char),1,stdout);
					break;
				}
			}
			if(arr[i].alpha == arr[i-1].alpha)
			{
				sum += arr[i].num;
				i++;
			}
			else if(arr[i].alpha != arr[i-1].alpha)
			{
				fwrite(&sum, sizeof(int),1,stdout);	
				fwrite(&(arr[i-1].alpha),sizeof(char),1,stdout);
				sum=0;
			}
		}
	} // end of while

	return 0;
}
