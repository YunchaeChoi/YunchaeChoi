#include <stdio.h>
#include <stdlib.h>

int main()
{
    size_t len=0;
    char* line=NULL;
    ssize_t nread;

    nread = getline(&line,&len,stdin);

    int i=0;
    while(i<nread)
    {
        printf("%c",line[i]);
        i++;
        if(i%10==0)
        {
            printf("\n");
        }
    }
}