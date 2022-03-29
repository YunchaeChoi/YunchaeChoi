#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	clockid_t types[8] = { CLOCK_REALTIME, CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC,
		CLOCK_MONOTONIC_COARSE, CLOCK_MONOTONIC_RAW, CLOCK_BOOTTIME, 
		CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID};
	struct timespec res;
	for(int i=0; i<8; i++) {
		if (clock_getres(types[i],&res) !=0 ) {
			printf("getres failed\n");
			exit(EXIT_FAILURE);
		} else {
			printf("%d: %ld %ld\n",i,(long)res.tv_sec,res.tv_nsec);
		}
	}

	return 0;
}
