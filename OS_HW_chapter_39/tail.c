#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "stack.h"

#define LINE_LEN 4096

void error_check_dp(DIR* dp) {
	if(dp == NULL) {
		fprintf(stderr, "opendir() failed.\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		fprintf(stderr, "wrong number of arguments.\n");
		exit(EXIT_FAILURE);
	}

	char buf[LINE_LEN];


	char *ptr = strtok(argv[1],"-");
	int n = atoi(ptr);

	char* path_name = argv[2];
	
//	int fd = open(path_name, O_RDONLY);
	FILE* fp = fopen(path_name, "r");
	for(int i=0;i<n;i++) {
		buf[0]='\0';
		fgets(buf,sizeof(buf), fp);
		buf[strlen(buf)-1] = '\0';
		push((char*)buf);
	}
	
	push("aa");
	push("bb");
	push("cc");

	printf("%s\n",pop());  
	printf("%s\n",pop());

	delAllNode();


	fclose(fp);
}
