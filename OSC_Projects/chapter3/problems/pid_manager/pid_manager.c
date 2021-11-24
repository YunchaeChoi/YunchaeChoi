#include <stdio.h>

#define MIN_PID 300
#define MAx_PID 5000

/* API */

int allocate_map(void); // Creates and initializes a data structure for representing pids; return -1 if unsuccessful, 1 if successful

int allocate_pid(void); // Allocates and returns a pid; return -1 if unable to allocate a pid (all pids are in use)

void release_pid(int pid); // Releases a pid

//int pid_use[4701]; /* set to 1 if pid is currently being used. index 0 means PID 300 and index 4700= PID 5000 */

int main()
{
    allocate_map()
}

int allocate_map(void)
{
    int pid_use[4701]={0,};
}
