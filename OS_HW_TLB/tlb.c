#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

#define BILLION 1E9
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
	

	int *a = (int*)calloc(PAGESIZE * NUMPAGES, sizeof(int));

	struct timespec begin, end;
	if( (clock_gettime(CLOCK_MONOTONIC,&begin)) == -1) {
		printf("clock_gettime error - begin\n");
		exit(EXIT_FAILURE);
	}

	for(int j=0; j< trials; j++) {
		for(int i=0; i< NUMPAGES * jump ; i += jump) {
			a[i] += 1;
		}
	}

	if( (clock_gettime(CLOCK_MONOTONIC, &end)) == -1 ) {
		printf("clock_gettime error - end\n");
		exit(EXIT_FAILURE);
	}
	free(a);


	float diff_nanos = (BILLION*(end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) ) / (trials * NUMPAGES);
//	diff_nanos /= (long)trials * (long)NUMPAGES ;

//	printf("%ld ns elapsed\n",time);
//	printf("%lf μs elapsed\n ",(double)time/1000); //micro
	printf("%d %f\n",NUMPAGES,diff_nanos);

	return 0;
}
