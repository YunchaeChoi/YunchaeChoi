#include <stdio.h>

int main(int agrc, char* argv[])
{
    FILE* fp;
    fp=fopen(argv[1],"r");
    unsigned char data;
    unsigned char page[256];
    long location;
    scanf("%ld",&location);
    fseek(fp, location, SEEK_SET);
    int i=0;
    for(int j=0;j<256;j++)
    {
        fread(&data,sizeof(unsigned char),1,fp);
        page[i++]=data;
    }
    for(int j=0;j<256;j++)
        printf("%hhu ",page[j]);
    printf("\n");
    fclose(fp);
    return 0;
}
