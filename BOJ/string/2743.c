#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* line=NULL;
    size_t len=0;
    ssize_t nread=0;
    nread = getline(&line,&len,stdin);
    printf("%d\n",(int)nread -1);
    free(line);
}
