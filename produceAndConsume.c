#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>



#define PRODUCER_NUM 5   // producer_worker number
#define CONSUMER_NUM 5   // consumer_worker number
#define BUCKET_SIZE  10  // max product number
#define TRUE 1

// gloval mutex
pthread_mutex_t mutex;
// conditional variable
pthread_cond_t cond;

typedef struct Bucket {
  int buffer[BUCKET_SIZE];
  int size;               // resource number left in the buffer
  int produce_index;      // produce index for the next product
  int consume_index;      // consume index for the next product
  int done;
} Bucket;

char* productsStrs[BUCKET_SIZE] = {"Product 1", "Product 2", "Product 3", "Product 4", "Product 5", 
                                    "Product 6", "Product 7", "Product 8", "Product 9", "Product 10" };

Bucket buckets;

int getRandomProductIndex() {
  return rand() % 10;
}

void displayAllProduct() {
  printf("Current Buffer: {");
  for (int i = 0; i < BUCKET_SIZE; i++) {
    if (buckets.buffer[i] == -1) {
      printf("NULL, ");
    }
    else {
      printf("%s, ", productsStrs[buckets.buffer[i]]);
    }
  }
  printf("}\n");
}


/*
  producer thread's work function.
  After the producer gets the mutex, check the number of products in the buffer:
  If the buffer is full, the producer waits conditionally
  If the buffer is not full, producer thread is produced, and then a contional signal is sent to wake up consumers and release the mutex

  In this problem, producer threads and consumer threads would sleep for a specified time so that the overall output of the 
  program would be clear for the users to analysis the multi-thread program running principle 
*/

void* producer_worker(void *arg) {
    int threadID = *(int *)arg;
    printf("producer thread: %d start working.\n", threadID);
    while (TRUE) {
      // if exceed 10 seconds, the producer thread function would just return.
      if (buckets.done == 1) {
        return NULL;
      }

      // sleep random second [0, 1] to make the thread runs slower so that actions could be reviewed clearly
    	usleep(100000 * (rand() % 10 + 1));
      
      // get the lock
      pthread_mutex_lock(&mutex);

      while (buckets.size == BUCKET_SIZE) {
        pthread_cond_wait(&cond, &mutex);
      }
      // producing
      buckets.produce_index = buckets.produce_index % BUCKET_SIZE;
      buckets.buffer[buckets.produce_index] = getRandomProductIndex();
      char* productName = productsStrs[buckets.buffer[buckets.produce_index]];
      printf("producer thread: %d produces product: %s and placed at: %d\n", threadID, productName, buckets.produce_index);
      buckets.produce_index++;
      //displayAllProduct();
      buckets.size++;

      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
void* consumer_worker(void *arg) {

  int threadID = *(int *)arg;
  printf("consumer thread: %d start working.\n", threadID);
  while (1) {
    // if exceed 10 seconds, the comsumer thread function would just return.
    if (buckets.done == 1) {
        return NULL;
    }
    // sleep random second [0, 2] to make the thread runs slower so that actions could be reviewed clearly
    usleep(100000 * (rand() % 20 + 1));
  
    pthread_mutex_lock(&mutex);
    while (buckets.size == 0) {
      pthread_cond_wait(&cond, &mutex);
    }


    // consuming
    buckets.consume_index = buckets.consume_index % BUCKET_SIZE;
    int product = buckets.buffer[buckets.consume_index];
    printf("consumer thread: %d consumes product: %s at index: %d\n", threadID, productsStrs[product], buckets.consume_index);
    buckets.buffer[buckets.consume_index] = -1;
    buckets.consume_index++;
    buckets.size--;
    //displayAllProduct();
    
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
// initialize the buffer which used for the producer and consumer threads
void initParameter() {
  for (int i = 0; i < BUCKET_SIZE; i++) {
    buckets.buffer[i] = -1;
  }
  buckets.consume_index = 0;
  buckets.produce_index = 0;
  buckets.done = 0;
  buckets.size = 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
}

// launch all pthreads with specified worker function
void launchPthread(pthread_t* threads, int threadSize, void*(*func)(void*), int* ids) {
  for (int i = 0; i < threadSize; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, func, (void*)&ids[i]);
  }
}

// release all threads by using pthread_join function
void releaseThreads(pthread_t* threads, int size) {
  for (int i = 0; i < size; i++) {
    pthread_join(threads[i], NULL);
  }
}

int main() {
  // initialize the time seed
  srand(time(NULL));
  // initialize all parameters
  initParameter();

  pthread_t comsumers[CONSUMER_NUM];
  pthread_t producers[PRODUCER_NUM];
  int consumers_id[CONSUMER_NUM];
  int producers_id[PRODUCER_NUM];

  launchPthread(producers, PRODUCER_NUM, producer_worker, producers_id);
  launchPthread(consumers, CONSUMER_NUM, consumer_worker, consumers_id);

  sleep(10);
  buckets.done = 1;

  // release all threads
  releaseThreads(producers, PRODUCER_NUM);
  releaseThreads(consumers, CONSUMER_NUM);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  return 0;
}
