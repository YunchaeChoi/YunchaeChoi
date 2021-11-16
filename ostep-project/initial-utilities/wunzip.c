#include <stdio.h>
#include <stdlib.h>

#define SIZE 255

int main(int argc, char** argv)
{
	if(argc==1)
	{
		printf("wunzip: file1 [file2 ...]\n");
		exit(1);
	}
	for(int j=0;j<argc-1;j++)
	{
		FILE *fp = fopen(argv[j+1],"r");
		int data;
		char c;
		while(1)
		{
			if(fread(&data,sizeof(int),1,fp) == 0 ) break;
			if(fread(&c,sizeof(char),1,fp) ==0 ) break;
			for(int i=0;i<data;i++)
			{
				printf("%c",c);
			}

		}
		fclose(fp);
	}
	return 0;
}
