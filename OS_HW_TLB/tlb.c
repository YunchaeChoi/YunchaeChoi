#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE);} while(0)

// how to view pdf file 
// $ wslview <filename>.pdf
int main(int argc, char* argv[]) {

	if(argc!=3) {
		printf("type 2 parameters");
		exit(EXIT_FAILURE);
	}

//	cpu_set_t cpuset;
//	pthread_t thread;
//
//	thread = pthread_self();
//
//	CPU_ZERO(&cpuset);
//	for(int i=0;i<8;i++) {
//		CPU_SET(i, &cpuset);
//	}
//
//	int s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
//	if(s!=0) {
//		handle_error_en(s, "pthread_setaffinity_np");
//	}
//	s = pthread_getaffinity_np(thread,sizeof(cpu_set_t), &cpuset);
//	if(s!=0) {
//		handle_error_en(s, "pthread_getaffinity_np");
//	}

	
	
	int NUMPAGES = atoi(argv[1]);
	int trials = atoi(argv[2]);
	int PAGESIZE = getpagesize();
	int jump = PAGESIZE / sizeof(int);
	
//	struct timespec begin, end;

	int *a = (int*)calloc(PAGESIZE * NUMPAGES, sizeof(int));


//	if( (clock_gettime(CLOCK_MONOTONIC,&begin)) == -1) {
//		printf("clock_gettime error - begin\n");
//		exit(EXIT_FAILURE);
//	}

	struct timeval begin, end;
	gettimeofday(&begin, NULL);

	for(int j=0; j< trials; j++) {
		for(int i=0; i< NUMPAGES * jump ; i += jump) {
			a[i] += 1;
		}
	}
	gettimeofday(&end, NULL);
	float time_diff = end.tv_sec-begin.tv_sec + 1e-6*(end.tv_usec - begin.tv_usec);

//	if( (clock_gettime(CLOCK_MONOTONIC, &end)) == -1 ) {
//		printf("clock_gettime error - end\n");
//		exit(EXIT_FAILURE);
//	}
	free(a);

	time_diff /= (float)trials;
//	time = time / trials;
//	printf("%ld ns elapsed\n",time);
//	printf("%lf μs elapsed\n ",(double)time/1000); //micro
	printf("%d %f\n",NUMPAGES,time_diff);

	return 0;
}
