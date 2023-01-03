#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITEMS 1000

struct candy{
    int id;
    char type[64];
};

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

struct candy buffer[MAX_ITEMS];
int selected;
int count = 0;
// used for producer and consumer
int in = 0;
int out = 0;
int proController;
int conController;
// for pack
int serial = 0;
int maxCandyInBox = 0;
int maximumCandy;
struct candy box[MAX_ITEMS][MAX_ITEMS];
struct candy candyArr[MAX_ITEMS];

// user input 
int takeBufferSize;
int candyTypes;

bool flag = false;
int pCnt;
int cCnt;
int ch;
int candyCounterINBuffer = 0;

// struct candy *c;

void *producer(void *arg)
{
    proController--;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    if(buffer[in].id == -1){
        buffer[in] = candyArr[selected];
        in = (in + 1) % takeBufferSize;
        count++;
        //printf("produced\n");
        conController++;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return NULL;
}

void *consumer(void *arg)
{
    conController--;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    struct candy item;
    item = buffer[out];
    // printf("%s ", item.type);

    // if(maxCandyInBox < maximumCandy){
    //     box[serial][maxCandyInBox] = item;
    //     maxCandyInBox++;
    // }
    // else{
    //     serial++;
    //     maxCandyInBox = 0;
    //     box[serial][maxCandyInBox] = item;
    //     maxCandyInBox++;
    // }

    box[serial][maxCandyInBox] = item;
    maxCandyInBox++;

    if (maxCandyInBox == maximumCandy) {
        box[serial][maxCandyInBox].id = -1;

        serial++;
        maxCandyInBox = 0;
        box[serial][maxCandyInBox] = item;
        maxCandyInBox++;
    }

    buffer[out].id = -1;
    buffer[out].type[0] = '\0';
    out = (out + 1) % MAX_ITEMS;
    count--;
    proController++;
    candyCounterINBuffer--;
    
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
        printf("No %d candy Name : ", i);
        scanf("%s", name);
        struct candy c;
        c.id = i;
        strcpy(c.type, name);
        candyArr[i] = c;
    }
}

int selectCandy()
{
    printf("Candy List:\n");
    for (int i = 0; i < candyTypes; i++)
    {
        printf("%d. %s\n", candyArr[i].id, candyArr[i].type);
    }

    int select;
    printf("Select any one candy from the list: ");
    scanf("%d", &select);
    if(select > ch){
        printf("Please write correct number: ");
        scanf("%d", &select);
    }
    else{
        return select;
    }
}

void pChoice();
void cChoice();
void showBuffer();
void typesOfCandy();
void candyInBOX();
void candyCountBuffer();

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
            box[i][j].type[0] = '\0';
        }
    }

    printf("Enter Assembly line size (BufferSize 1 - 1000): ");
    scanf("%d", &takeBufferSize);
    createCandy();

    proController = takeBufferSize;

    //printf("%d", MAX_ITEMS);

    int input;
    printf("\nWelcome to Amherst Candy Factory\n");
    printf("1. Candy Produce\n");
    printf("2. Candy Consume\n");
    printf("3. Show Assembly Line \n");
    printf("4. Show Available Candy \n");
    printf("5. Show Available Packages \n");
    printf("0. Exit the factory\n");

    printf("\nEnter your option: ");
    scanf("%d", &input);

    while(input != 0){
        if(input == 1){
            int numProducer;
            printf("How many candy you want to produce: ");
            scanf("%d", &numProducer);
            printf("\n");
            ch = numProducer;
            for(int i = 0; i < numProducer; i++){
                if(proController >= 1){
                    if(numProducer <= takeBufferSize){
                    selected = selectCandy();
                    pthread_create(&producer_t[i], NULL, (void *)producer, NULL);
                    printf("Producer %d: produced %s candy", i, candyArr[selected].type);
                    printf(" Stored at %d of the Buffer\n", in);
                    }
                    else{
                        printf("You can maximum produce at this buffer: %d\n", takeBufferSize);
                        break;
                    }
                }
                else{
                    printf("buffer is full\n");
                    break;
                }
            }
        }
        else if (input == 2)
{
    candyCountBuffer();
    int numConsumer;
    if (conController > 0)
    {
        printf("\nHow many candy you want to consume: ");
        scanf("%d", &numConsumer);
        if (numConsumer <= candyCounterINBuffer)
        {
            printf("How many candy you want in a Box: ");
            scanf("%d", &maximumCandy);
            for (int i = 0; i < numConsumer; i++)
            {
                if (conController > 0)
                {
                    pthread_create(&consumer_t[i], NULL, (void *)consumer, NULL);
                    if (i == 0)
                    {
                        printf("%s ", buffer[0].type);
                        printf("consumed from Buffer %d\n", out);
                    }
                    else
                    {
                        printf("%s ", buffer[i].type);
                        printf("consumed from Buffer %d\n", i);
                    }
                }
                else
                {
                    printf("buffer is empty\n");
                    break;
                }
            }
        }
        else
        {
            printf("Not enough candy in Buffer\n");
            printf("Available candy now in Buffer -> %d\n", candyCounterINBuffer);
        }
    }
    else
    {
        printf("buffer is empty\n");
    }
    
        }
        else if(input == 3){
            showBuffer();
        }
        else if(input == 4){
            typesOfCandy();
        }
        else if(input == 5){
            candyInBOX();
        }

        printf("\nWelcome to Amherst Candy Factory\n");
        printf("1. Candy Produce\n");
        printf("2. Candy Consume\n");
        printf("3. Show Assembly Line \n");
        printf("4. Show Available Candy \n");
        printf("5. Show Available Packages \n");
        printf("0. Exit the factory\n");
        printf("Enter your option: ");
        scanf("%d", &input);
    }

    //pthread_join(producer_t, NULL);
    //pthread_join(consumer_t, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}

/*
void pChoice(){
    int numProducer;
    printf("How many candy you want to produce: ");
    scanf("%d", &numProducer);
    ch = numProducer;
    for (int i = 0; i < numProducer; i++)
    {
        if (proController >= 1)
        {
            selected = selectCandy();
            pthread_create(&producer_t[i], NULL, (void *)producer, NULL);
            printf("Producer %d: produced %s candy", i, candyArr[selected].type);
            printf(" Stored at %d of the Buffer\n", in);
        }
        else
        {
            printf("buffer is full\n");
            break;
        }
    }
}

void cChoice(){
    int numConsumer;
    if (conController > 0)
    {
        printf("\nHow many candy you want to consume: ");
        scanf("%d", &numConsumer);
        printf("How many candy you want in a Box: ");
        scanf("%d", &maximumCandy);
        for (int i = 0; i < numConsumer; i++)
        {
            if (conController > 0)
            {
                pthread_create(&consumer_t[i], NULL, (void *)consumer, NULL);
                if (i == 0)
                {
                    printf("%s ", buffer[0].type);
                    printf("consumed from Buffer %d\n", out);
                }
                else
                {
                    printf("%s ", item.type);
                    printf("consumed from Buffer %d\n", out - 1);
                }
            }
            else
            {
                printf("buffer is empty\n");
                break;
            }
        }
    }
    else
    {
        printf("buffer is empty\n");
    }
}
*/

void candyInBOX()
{
    printf("Total Box: %d\n", serial);
    if (serial > 0)
    {
        for (int i = 0; i < serial; i++)
        {
            printf("Box %d contains:\n", i);

            for (int j = 0; j < maximumCandy; j++)
            {
                if(box[i][j].id != -1){
                    int idx = box[i][j].id;
                    printf("%d %s\n", j, candyArr[idx].type);
                }
            }
        }
    }
    else
    {
        printf("No candy in Box\n");
    }
}

void showBuffer(){
    printf("Available Candies in the Assembly Line:\n");
    for (int i = 0; i < takeBufferSize; i++)
    {
        if (buffer[i].id != -1)
        {
            printf("%d. %s\n", i, buffer[i].type);
        }
        else if(buffer[i].id == -1){
            continue;
        }
        else
        {
            printf("Buffer is empty.\n");
            break;
        }
    }
    // for(int i = 0; i < takeBufferSize; i++){
    //     printf("%d. %s\n", buffer[i].id, buffer[i].type);
    // }
}

void candyCountBuffer(){
    for (int i = 0; i < takeBufferSize; i++)
    {
        if (buffer[i].id != -1)
        {
            candyCounterINBuffer++;
        }
        else if(buffer[i].id == -1){
            continue;
        }
        else
        {
            break;
        }
    }
}

void typesOfCandy(){
    for (int i = 0; i < candyTypes; i++)
    {
        if (candyArr[i].type[2] != '\0')
        {
            printf("%d. %s\n", i, candyArr[i].type);
        }
    }
}