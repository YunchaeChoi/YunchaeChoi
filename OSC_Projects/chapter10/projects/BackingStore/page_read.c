#include <stdio.h>

int main(int agrc, char* argv[])
{
    FILE* fp;
    fp=fopen(argv[1],"r");
    unsigned char data;
    unsigned char page[256];
    for(int i=0;i<256;i++)
    {
        fread(&data,sizeof(unsigned char),1,fp);
        page[i]=data;
    }
    for(int i=0;i<256;i++)
    {
        printf("%d: %hhu\n",i+1,page[i]);
    }
    return 0;
}
