/*
 * gcc -o cp cp.c -Wall -Werror
 * try "strace ./cp" to which syscalls are called to run this executable file
 * */





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    char* input;
    char* output;
    size_t len_i;
    size_t len_o;
    printf("which file do you want to copy?: ");
    ssize_t nread=getline(&input,&len_i,stdin);
    if(nread==-1)
    {
        printf("input: getline error\n");
        exit(EXIT_FAILURE);
    }
    printf("what do you want to name an new output file?: ");
    nread=getline(&output,&len_o,stdin);
    if(nread==-1)
    {
        printf("output: getline error\n");
        exit(EXIT_FAILURE);
    }
    char path_input[20];
    char path_output[20];
    sprintf(path_input,"./%s",input);
    sprintf(path_output,"./%s",output);

    char* newline = strchr(path_input,'\n');
    if(newline)
    {
        *newline='\0';
    }
    newline=strchr(path_output,'\n');
    if(newline)
    {
        *newline='\0';
    }
    printf("%s\n",path_input);
    int file_check = access(path_input,F_OK); // RTFM
    if(file_check!=0)
    {
        printf("input file doensn't exist\n");
        exit(EXIT_FAILURE);
    }
    file_check=access(path_input,R_OK);
    if(file_check==-1)
    {
        printf("Permission denied: reading\n");
        exit(EXIT_FAILURE);
    }
    FILE* fr;
    fr = fopen(path_input,"r");
    if(!fr)
    {
        printf("Error: open\n");
        exit(EXIT_FAILURE);
    }
    file_check=access(path_output,F_OK);
    if(file_check==0)
    {
        printf("writing file already exists.\n");
        exit(EXIT_FAILURE);
    }
    FILE* fw;
    fw=fopen(path_output,"w");
    char buffer[255];
    while(!feof(fr))
    {
        char* eof = fgets(buffer,sizeof(buffer),fr);
        if(!eof)
        {
            break; // EOF
        }
        fputs(buffer,fw);
        //printf("%s",buffer);
        memset(buffer,0,sizeof(buffer));
    }
    fclose(fw);
    printf("Write completion\n");
    return 0;
}
