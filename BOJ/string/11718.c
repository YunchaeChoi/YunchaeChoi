#include <stdio.h>
#include <stdlib.h>

int main()
{
    size_t len=0;
    char* line=NULL;
    ssize_t nread;

    while((nread=getline(&line,&len,stdin))!=-1)
    {
        printf("%s",line);
    }
    free(line);
    return 0;
}
