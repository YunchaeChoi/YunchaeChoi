#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
	char* file = argv[1];
	struct stat st;

	if(stat(file, &st)==-1) {
		fprintf(stderr, "stat() error\n");
		exit(EXIT_FAILURE);
	}

	printf("ID of device containing file: %ld\n", st.st_dev);
	printf("Inode number: %ld\n", st.st_ino);
	printf("File type and mode: \n");
	printf("number of hard links: %ld\n", st.st_nlink);
	printf("UID of owner: %d\n",st.st_uid);
	printf("GID of owenr: %d\n",st.st_gid);



	printf("size: %ld\n", st.st_size);
	printf("number of blocks allocated: %ld\n", st.st_blocks);
	return 0;

}
