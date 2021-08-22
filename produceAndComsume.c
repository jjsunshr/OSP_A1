#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>



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


int main() {

  return 0;
}
