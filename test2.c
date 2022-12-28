#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#define MAX_ITEMS 1000

struct candy{
    int id;
    char *type[64];
};

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

struct candy buffer[MAX_ITEMS];
int count = 0;
int in = 0;
int out = 0;
// for pack
int serial = 0;
int maxCandyInBox = 0;
struct candy pack[MAX_ITEMS][MAX_ITEMS];
struct candy candyArr[MAX_ITEMS];

// user input 
int takeBufferSize;
int candyTypes;


void *producer(void *arg)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    struct candy c;

    buffer[in] = c;
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

    struct candy item = buffer[out];
    printf("%s ", &item.type);

    buffer[out].id = -1;
    buffer[out].type[0] = '\0';
    out = (out + 1) % MAX_ITEMS;
    count--;
    printf("consumed\n");
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return NULL;
}

void createCandy()
{
    printf("Enter how many type of candy: ");
    scanf("%d", &candyTypes);
    char name[64];
    for (int i = 0; i < candyTypes; i++)
    {
        printf("No %d candy Name : ", i+1);
        scanf("%s", name);
        struct candy c;
        c.id = i+1;
        c.type[0] = name;
        candyArr[i] = c;
    }
}

int selectCandy()
{
    for (int i = 1; i <= candyTypes; i++)
    {
        printf("%d. %s\n", candyArr[i].id, &candyArr[i].type);
    }

    int select;
    printf("Select any one candy from the list: ");
    scanf("%d", &select);
    return select;
}

int main()
{
    sem_init(&empty, 0, MAX_ITEMS);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t producer_t[MAX_ITEMS], consumer_t[MAX_ITEMS];

    // pthread_create(&producer_t, NULL, producer, NULL);
    // pthread_create(&consumer_t, NULL, consumer, NULL);

    for (int i = 0; i < MAX_ITEMS; i++) { 
        buffer[i].id = -1; 
        buffer[i].type[0] = '\0';
    }

    for(int i = 0; i < MAX_ITEMS; i++){
        for(int j = 0; j < MAX_ITEMS; j++){
            pack[i][j].type[0] = '\0';
        }
    }

    printf("Enter Assembly line size (BufferSize 1 - 1000): ");
    scanf("%d", &takeBufferSize);
    createCandy();

    //printf("%d", MAX_ITEMS);

    int input;
    printf("Do you want to produce or consume?\n");
    printf("1. Produce\n");
    printf("2. Consume\n");
    printf("3. Show Assembly Line \n");
    printf("0. Exit the factory\n");

    scanf("%d", &input);

    while(input != 0){
        if(input == 1){
            int numProducer;
            scanf("%d", &numProducer);
            for(int i = 0; i < numProducer; i++){
                if(count <= MAX_ITEMS){
                    int selected = selectCandy();
                    pthread_create(&producer_t[i], NULL, producer, NULL);
                    printf("Producer %d: produced %s candy", i, &candyArr[selected].type);
                    // can be update here
                    printf("Stored at %d of the Buffer\n", in);
                }
                else{
                    printf("buffer is full\n");
                }
            }
        }
        else if(input == 2){
            int numConsumer;
            scanf("%d", &numConsumer);
            for(int i = 0; i < numConsumer; i++){
                if(count == 0){
                    printf("buffer is empty\n");
                }
                else{
                    pthread_create(&consumer_t[i], NULL, consumer, NULL);
                }
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