#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


#define TRUE 1
// number of the fork
#define FORK_NUM 5
// 1 second = 10 * 0.1 second
#define SECOND_UNIT 10

// fork mutex array, there would be FORK_NUM fork mutex
pthread_mutex_t forks[FORK_NUM];

// index philosopger with corresponding forks, 0 with 4 and 0 fork
int philosopgerChops[FORK_NUM][2] = {{4, 0}, {0, 1}, {1, 2}, {2, 3}, {3, 4}};
// used time for dinner, make fair for the dinner time
double eatTimes[FORK_NUM] = {0,0,0,0,0};
int done = 0;

// generate random time between 1 and 10
double generateRandomTime() {
    return (double)(rand() % 10 + 1);
    
}


int main(){

    return 0;

}
