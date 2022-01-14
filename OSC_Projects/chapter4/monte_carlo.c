/* 4.24 */
/* radius = 1 */

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int points_in_circle=0; // global variable to count the number of points in the circle

void* parent(void* args)
{

}

void* circle(void* args)
{

}


int main()
{
    srand(time(NULL));
    int num_of_threads; // separate thread to generate a number of random points and count the number of points that occur within the circle and store that result in a gloabl variable
    int num_of_points;  // how many points will a seperate thread create?

    scanf("%d %d",&num_of_threads,&num_of_points);

    pthread_t tid[num_of_threads];


}
