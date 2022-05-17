#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define SIZE 4096
#define TRUE 1
#define FALSE 0

char open_path[SIZE];
int follow_link = FALSE; // default if false

void error_check_dp(DIR* dp) {
	if(dp == NULL) {
		fprintf(stderr, "opendir() failed.\n");
		exit(EXIT_FAILURE);
	}
}

void error_check_getcwd(char path[]) {
	if(path == NULL) {
		fprintf(stderr, "getcwd() failed.\n");
		exit(EXIT_FAILURE);
	}
}

void recursive_search(char* start) { // start : starting path
	char path[SIZE];
	struct dirent* d_entry;
	DIR* dp;

	struct stat st;

	lstat(start, &st);

	if(S_ISREG(st.st_mode)) { // regular file
		return;
	}
	if(S_ISLNK(st.st_mode)) {
		if(!follow_link)
			return;
	}
	else {
		dp = opendir(start);
	}

	dp = opendir(start);
	error_check_dp(dp);

	while((d_entry = readdir(dp))) {
		if(strcmp(d_entry->d_name,".") !=0 && strcmp(d_entry->d_name, "..") != 0) {
			printf("%s/%s\n", start, d_entry->d_name);
			strcpy(path, start);
			if(strcmp(path,"/")!=0) {
				strcat(path, "/");
			}
			strcat(path, d_entry->d_name);
			stat(path, &st);

			recursive_search(path);
		}
	}

	closedir(dp);
}

int main(int argc, char* argv[]) {

	if(argc == 1) {
		getcwd(open_path, PATH_MAX);
		error_check_getcwd(open_path);
	}
	else if(argc == 2) {
		strcpy(open_path, argv[1]);
	}
	else {
		perror("wrong number of arguments\n");
	}
	printf("%s\n", open_path);

	recursive_search(open_path);

}
