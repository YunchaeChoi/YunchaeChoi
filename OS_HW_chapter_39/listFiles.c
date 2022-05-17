#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

#define SIZE 4096
#define TRUE 1
#define FALSE 0

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

char* check_permission(int permission) {
	char output[] = "----------";

	if (!S_ISREG(permission)) { // not regular file
		if(S_ISDIR(permission)) {
			output[0] = 'd';
		}
		if(S_ISLNK(permission)) {
			output[0] = 'l';
		}
	}

	if((permission & S_IREAD) != 0) {
		output[1] = 'r';
	}
	if((permission & S_IRGRP) != 0) {
		output[4]= 'r';
	}
	if((permission & S_IROTH) != 0) {
		output[7] = 'r';
	}

	if((permission & S_IWUSR) != 0) {
		output[2] = 'w';
	}
	if((permission & S_IWGRP) != 0) {
		output[5] = 'w';
	}
	if((permission & S_IWOTH) != 0) {
		output[8] = 'w';
	}

	if((permission & S_IXUSR) != 0) {
		output[3] = 'x';
	}
	if((permission & S_IXGRP) != 0) {
		output[6] = 'x';
	}
	if((permission & S_IXOTH) != 0) {
		output[9] = 'x';
	}

	char* ret = &output[0];
	return ret;
}

int main(int argc, char* argv[]) {
	DIR *dp;
	struct dirent *d_entry;
	char cur_path[SIZE];
	int option= FALSE;
	char* path_name;

	if(argc == 1) {
		getcwd(cur_path, PATH_MAX);
		error_check_getcwd(cur_path);

		dp = opendir(cur_path);
		error_check_dp(dp);
		path_name = cur_path;
	}

	if(argc==2) {
		if(strcmp(argv[1],"-l")!=0) { // if not '-l' option
			dp = opendir(argv[1]);
			error_check_dp(dp);
			path_name = argv[1];
		}
		else { // ex) ./a.out -l
			option = TRUE;
			getcwd(cur_path,PATH_MAX);
			error_check_getcwd(cur_path);

			dp = opendir(cur_path);
			error_check_dp(dp);
			path_name =cur_path;
		}
	}
	
	if(argc == 3) {
		if(strcmp(argv[1],"-l")!=0) {
			fprintf(stderr, "wrong option.\n");
			exit(EXIT_FAILURE);
		}
		else { // ex) ./a.out -l /bin
			option = TRUE;
			dp = opendir(argv[2]);
			error_check_dp(dp);
			path_name = argv[2];
		}
	}


	
	if(option == FALSE) {
		while( d_entry = readdir(dp) ) {
			printf("%s\n",d_entry->d_name);
		}
	}
	else {
		long int total_blocks = 0;
		struct stat st;
		struct passwd *pws;
		struct group *g;

		while( d_entry = readdir(dp) ) {
			char stat_path_name[SIZE];
			stat_path_name[0] = '\0';
			strcat(stat_path_name, path_name);
			if (strcmp(path_name,"/") !=0) { // if not root directory, concat /
				strcat(stat_path_name,"/");
			}
			strcat(stat_path_name, d_entry->d_name);
//			printf("stat_path_name: %s\n", stat_path_name);

			if(stat(stat_path_name, &st)==-1) {
				fprintf(stderr, "stat() failed.\n");
				printf("errno: %d\n", errno);
				exit(EXIT_FAILURE);
			}
			total_blocks += st.st_blocks;
			uid_t owner = st.st_uid;
			gid_t group = st.st_gid;
			mode_t permission = st.st_mode;

			pws = getpwuid(owner);
			g = getgrgid(group);

			printf("%s user: %s, group: %s, bytes used: %ld  %s\n", check_permission(permission), 
					pws->pw_name, g->gr_name, st.st_size, d_entry->d_name);
		}
		printf("total: %ld\n", total_blocks /2); // linux uses 1,024-byte block
	}

	closedir(dp);
	return 0;
}
