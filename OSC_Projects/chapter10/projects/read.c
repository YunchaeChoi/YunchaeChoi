#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>

#define SIZE 256


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

	signed char value;
	printf("hi\n");
	lseek(fd,47 * 256 + 75,SEEK_SET);
	if(read(fd,&value, 1) == -1 )
	{
		printf("read fail!!\n");
	}

	printf("value: %d\n",value);
	close(fd);
	return 0;
}
