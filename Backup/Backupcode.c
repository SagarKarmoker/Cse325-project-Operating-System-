/*
#include <stdio.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <unistd.h> 
  
sem_t mutex; 
  
void* producer(void* arg) 
{ 
    sem_wait(&mutex); 
    printf("Producer is producing items...\n"); 
    sleep(2); 
    sem_post(&mutex); 
} 
  
void* consumer(void* arg) 
{ 
    sem_wait(&mutex); 
    printf("Consumer is consuming items...\n"); 
    sleep(2); 
    sem_post(&mutex); 
} 
  
int main() 
{ 
    sem_init(&mutex, 0, 1); 
    pthread_t producer_thread, consumer_thread; 
    pthread_create(&producer_thread, NULL, producer, NULL); 
    pthread_create(&consumer_thread, NULL, consumer, NULL); 
    pthread_join(producer_thread, NULL); 
    pthread_join(consumer_thread, NULL); 
    sem_destroy(&mutex); 
    return 0; 
}
*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_ITEMS 5
int buffer[MAX_ITEMS];
int count = 0;
int in = 0;
int out = 0;
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void *producer(void *arg)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = rand();
    in = (in + 1) % MAX_ITEMS;
    count++;
    printf("produced\n");
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return NULL;
}

void *consumer(void *arg)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    int item = buffer[out];
    buffer[out] = -1;
    out = (out + 1) % MAX_ITEMS;
    count--;
    printf("consumed\n");
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return NULL;
}

int main()
{
    sem_init(&empty, 0, MAX_ITEMS);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t producer_t, consumer_t;

    // pthread_create(&producer_t, NULL, producer, NULL);
    // pthread_create(&consumer_t, NULL, consumer, NULL);

    for(int i = 1; i <= MAX_ITEMS; i++){
        buffer[i-1] = -1;
    }

    printf("%d", MAX_ITEMS);

    int input;
    printf("Do you want to produce or consume?\n");
    printf("1. Produce\n");
    printf("2. Consume\n");
    scanf("%d", &input);

    while(input != 0){
        if(input == 1){
            if(count <= MAX_ITEMS){
                pthread_create(&producer_t, NULL, producer, NULL);
            }
            else{
                printf("buffer is full\n");
            }
        }
        else if(input == 2){
            if(count == 0){
                printf("buffer is empty\n");
            }
            else{
                pthread_create(&consumer_t, NULL, consumer, NULL);
            }
        }
        else if(input == 3){
            for(int i= 0; i < MAX_ITEMS; i++){
                printf("%d. %d\n", i, buffer[i]);
            }
        }

        scanf("%d", &input);
    }

    //pthread_join(producer_t, NULL);
    //pthread_join(consumer_t, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}