#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

/* link with -lm -pthread */

#define TRUE 1
#define FALSE 0

int is_in_circle(double r, double x, double y)
{
    if(pow(x,2)+pow(y,2) <= pow(r,2)) // dot (x,y) is in the circle. 원의 경계도 포함시켰기 때문에 <= 사용함.
    {
        return TRUE;
    }
    else return FALSE;
}

void* main_thread(void *data)
{

}

void* monte_carlo(void *data)
{
//    int point_in_circle=0;
//    double r; // a radius of the circle
//    int i=0;
//    double x,y;
//    for(;i<num_of_random_points;i++)
//    {
//
//    }
    
}


int main(int argc, char* argv[])
{
    int num_of_threads, num_of_random_points;
    printf("number of threads:  ");
    scanf("%d",&num_of_threads);
    printf("number of random points per thread: ");
    scanf("%d",&num_of_random_points);

    pthread_t tid[num_of_threads];

}
