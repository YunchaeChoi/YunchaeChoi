#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char* line = NULL;
    size_t len=0;
    ssize_t nread=0;

    nread = getline(&line,&len,stdin);
    int i=0;
    printf("%ld",nread-1);
    free(line);
}