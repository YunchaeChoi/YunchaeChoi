#include <stdio.h>
#define SIZE 255

int main(int argc, char** argv)
{
	FILE *fp = fopen(argv[1],"r");
	int data;
	char c;
	while(1)
	{
        if( fread(&data,sizeof(int),1,fp)==0 )
            break;
        printf("%d\n",data);
	}
    printf("\n");
	fclose(fp);
	return 0;
}
