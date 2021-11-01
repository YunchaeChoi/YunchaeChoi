#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255

int main(int argc, char** argv)
{
	FILE* fp;
	char buffer[SIZE];
	for(int i=1;i<argc;i++)
	{
		if((fp=fopen(argv[i],"r"))==NULL)
		{
			printf("wzip: open file failed\n");
			exit(1);
		}
		fgets(buffer,sizeof(buffer),fp);
		char temp[SIZE];
		int j=0, cnt=0;
		printf("%d\n",strlen(buffer));
		while(j<strlen(buffer))
		{
			if(cnt==0) // when buffer[j] is the first character of the input string
			{
				temp[cnt]=buffer[j];
				j++;cnt++;
			}
			else
			{
				if(buffer[j]==temp[cnt-1]) // if current character of input string is same as temp[cnt-1]
				{
					temp[cnt]=buffer[j];
					j++;cnt++;
				}
				else
				{
					printf("%s\n",temp);
					cnt=0;
					temp[0]='\0';
				}
			}
		}

		
	}
	return 0;
}
