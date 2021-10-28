#include <stdio.h>
#include <stdlib.h>

#define SIZE 255

int main(int argc, char** argv)
{
	char buffer[SIZE];
	for(int i=0;i<argc-1;i++)
	{
		FILE *fp = fopen(argv[i+1],"r");
		if(fp==NULL)
		{
			printf("wcat: cannot open file\n");
			exit(1);
		}
		while(fgets(buffer,SIZE,fp)!=NULL)
		{
			printf("%s",buffer);
			buffer[0]='\0';
		}
		fclose(fp);
	}
	return 0;
}
