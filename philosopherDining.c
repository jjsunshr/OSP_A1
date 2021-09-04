#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>


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

// find weather the index time is less than the average time
int lessThanAvgTime(int index) {
    double sum = 0;
    for (int i = 0; i < FORK_NUM; i++) {
        sum += eatTimes[i];
    }
    return eatTimes[index] < (sum / FOPEN_MAX);
}

// thread worker function
void *eat_think(void *arg) {
	// get the index of the philosophers
    int index = *(int *)arg;
	// get the left and right fork
	int leftFork  = philosopgerChops[index][0];
    int rightFork = philosopgerChops[index][1];

	while (TRUE) {
        // thread return
        if (done == 1) return NULL;
		
		// get the sleep time for the left fork
        double time = generateRandomTime();
		printf("Philosopher %d is thinking %.3fs\n", index, time / SECOND_UNIT);
        usleep(100000 * time);
         
		pthread_mutex_lock(&forks[leftFork]); 
		printf("Philosopher %d gets the fork %d\n", index, leftFork);
		// get the sleep time for the right fork
        time = generateRandomTime();
		printf("Philosopher %d is thinking %.3fs\n", index, time / SECOND_UNIT);
        usleep(100000 * time);
		
		// try to mutex the lock 
        if (pthread_mutex_trylock(&forks[rightFork]) == EBUSY){ 
			// if can not get the right fork, then release the left fork
			pthread_mutex_unlock(&forks[leftFork]); 
            printf("Philosopher %d put fork %d down\n", index, leftFork);
			continue;
		}
        else {
			// get the right fork
            printf("Philosopher %d gets the fork %d\n", index, rightFork);
        }
			
		// to make all philosopger fair for eating dinner, 
		// a array would be used to record the dinner time, 
		// if the current philosopger dinner time less than the average time
		// then he can eat dinner
        /*if (lessThanAvgTime(index) || eatTimes[index] == 0) {
            time = generateRandomTime();
            usleep(100000 * time);
            eatTimes[index] += time; 
            printf("Philosopher %d is eating %.3fs\n", index, time / SECOND_UNIT);
        }*/
		
		
	}
}

void displayAllEatTime() {
    printf("\n\nSummary.\n");
    for (int i = 0; i < FORK_NUM; i++) {
        printf("Philosopher %d eat %.3fs\n", i, eatTimes[i]);
    }
}

int main(){
    pthread_t philosophers[FORK_NUM];
    int threadIDs[FORK_NUM];
    srand(time(NULL));
	
	// init the mutex
	for (int i = 0; i < FORK_NUM; i++) {
        pthread_mutex_init(&forks[i],NULL);
    }

    sleep(10);
    done = 1;
	
	// release all threads
    for (int i = 0; i < FORK_NUM; i++) {
        pthread_join(philosophers[i], NULL);
    }

	// display all eat time for all philosopgers
    displayAllEatTime();
	return 0;
}