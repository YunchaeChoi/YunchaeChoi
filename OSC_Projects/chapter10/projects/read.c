#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>

#define SIZE 255


int main(int argc, char** argv)
{
	int fd;
	fd = open(argv[1],O_RDONLY);
	if(fd==-1)
	{
		printf("open fail\n");
		exit(EXIT_FAILURE);
	}
	printf("opened!\n");

	unsigned char value[256];
	printf("hi\n");
	if(read(fd,value, 256) == -1 )
	{
		printf("read fail!!\n");
	}

	for(int i=0;i<256;i++)
		printf("%hhu",value[i]);
    printf("\n");
	close(fd);
	return 0;
}
