#include <stdio.h>
#define SIZE 255

int main(int argc, char** argv)
{
	FILE *fw = fopen(argv[1],"r");
	int data;
	char c;
	while(1)
	{
		if(fread(&data,sizeof(int),1,fw) == 0 ) break;
			printf("%d",data);
		if(fread(&c,sizeof(char),1,fw) == 0 ) break;
			printf("%c",c);
	}
	fclose(fw);
	return 0;
}
