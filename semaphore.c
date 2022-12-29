#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

// Maximum buffer size
#define MAX_BUFF_SIZE 100

// Number of producers
int num_producers;

// Number of consumers
int num_consumers;

// Number of types of candies
int num_candies;

// Buffer
int buffer[MAX_BUFF_SIZE];

// Buffer index
int buffer_index;

// Mutex lock for buffer access
pthread_mutex_t buff_mutex;

// Semaphore for signaling producer-consumer
sem_t prod_cons;

//Producer thread
void* producer(void *param) {
  int candy_type = *((int *)param);
  int i;
  for (i = 0; i < 10; i++) {
    // Lock the buffer
    pthread_mutex_lock(&buff_mutex);
    // Insert candy into buffer
    buffer[buffer_index++] = candy_type;
    // Signal consumer
    sem_post(&prod_cons);
    // Unlock the buffer
    pthread_mutex_unlock(&buff_mutex);
  }
  pthread_exit(0);
}

//Consumer thread
void* consumer(void *param) {
  int i;
  for (i = 0; i < num_candies; i++) {
    // Wait for a candy
    sem_wait(&prod_cons);
    // Lock the buffer
    pthread_mutex_lock(&buff_mutex);
    // Take a candy from buffer
    int candy_type = buffer[--buffer_index];
    // Unlock the buffer
    pthread_mutex_unlock(&buff_mutex);
    printf("Box %d: Candy type %d\n", i, candy_type);
  }
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  // Initialize the arguments
  num_producers = atoi(argv[1]);
  num_consumers = atoi(argv[2]);
  num_candies = atoi(argv[3]);

  // Initialize the buffer
  buffer_index = 0;

  // Initialize the mutex lock
  pthread_mutex_init(&buff_mutex, NULL);

  // Initialize the semaphore
  sem_init(&prod_cons, 0, 0);

  // Create producer threads
  pthread_t producers[num_producers];
  int i;
  for (i = 0; i < num_producers; i++) {
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    pthread_create(&producers[i], NULL, producer, arg);
  }

  // Create consumer threads
  pthread_t consumers[num_consumers];
  for (i = 0; i < num_consumers; i++) {
    pthread_create(&consumers[i], NULL, consumer, NULL);
  }

  // Wait for producer threads
  for (i = 0; i < num_producers; i++)
    pthread_join(producers[i], NULL);

  // Wait for consumer threads
  for (i = 0; i < num_consumers; i++)
    pthread_join(consumers[i], NULL);

  // Destroy the semaphore
  sem_destroy(&prod_cons);

  // Destroy the mutex lock
  pthread_mutex_destroy(&buff_mutex);
  return 0;
}