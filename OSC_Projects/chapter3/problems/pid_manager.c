#include <stdio.h>

#define MIN_PID 300
#define MAX_PID 5000

#define SIZE MAX_PID-MIN_PID+1

#define FULL

/* global variables */

int bitmap[SIZE]; // value of 1 indicates that the process id is currently in use

int cur; // cursor pointing at the smallest pid available

/* fuctions */

int allocate_map(void); // Creates and initializes a data structure for representing pids; return -1 if unsuccessful, 1 if successful

int allocate_pid(void); // Allocates and returns a pid; return -1 if unable to allocate a pid (all pids are in use)

void release_pid(int pid); // Releases a pid

void init();

void find_min_pid();

//int pid_use[4701]; /* set to 1 if pid is currently being used. index 0 means PID 300 and index 4700= PID 5000 */

int main()
{
    
}

void find_min_pid()
{
    for(int i=0;i<SIZE;i++)
    {
        if(bitmap[i]==0) // pid available
        {
            cur=i;
            return;
        }
    }
    cur=FULL;
}

int allocate_map()
{

}

int allocate_pid()
{
    int pid;
    if(cur==MAX_PID+1)
    {
        find_min_pid();
        if(cur==FULL)
            return -1;
    }
    pid=cur;
    find_min_pid();
    return pid;
}

void release_pid(int pid)
{
    bitmap[pid-MIN_PID]=0;
    find_min_pid();
}

void init()
{
    for(int i=0;i<SIZE;i++)
    {
        bitmap[i]=0;
    }
    find_min_pid();
}
