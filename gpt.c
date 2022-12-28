#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 1000
#define maxItem 1000

struct candy
{
    int id;
    char *type;
};

sem_t semFull;
sem_t semEmpty;
sem_t mutex_var;
// pthread_mutex_t mutex_var;

// variable
int in = 0;
int out = 0;
bool flag = false;

int pCnt;
int cCnt;

int proController;
int conController;

// user input variable
int takeBufferSize;
int candyTypes;
int numBoxs;
int boxSize;
int currentCandyInBox = 0;
struct candy candyArr[maxItem];
struct candy buffer[BUFFER_SIZE];
struct candy candyBox[maxItem];

void createCandy()
{
    printf("Enter how many type of candy: ");
    scanf("%d", &candyTypes);
    char *name = malloc(maxItem * sizeof(char));
    for (int i = 1; i <= candyTypes; i++)
    {
        printf("No %d candy Name : ", i);
        scanf("%s", &name);
        struct candy *c = malloc(sizeof(struct candy));
        c->id = i;
        c->type = name;
        candyArr[i].id = c->id;
        candyArr[i].type = c->type;
    }

    // print candy type
    // for (int i = 1; i <= candyTypes; i++)
    // {
    //     printf("%d %s\n", candyArr[i].id, candyArr[i].type);
    // }
}
void *candyProducer(void *arg)
{
    proController--;
    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutex_var);

    struct candy c;
    buffer[in] = c;
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex_var);
    conController++;

    // int cid = rand() % candyTypes;
    // cid = cid + 1;

    // c.id = cid;
    // printf("producer %d produced %s\n", id + 1, candyArr[c.id].type);
    sem_post(&semFull);
    pthread_exit(NULL);
}
void *candyConsumer(void *arg)
{
    conController--;
    sem_wait(&semFull);
    pthread_mutex_lock(&mutex_var);

    if (currentCandyInBox == takeBufferSize)
    {
        printf("Can not add candy to box because box is full\n");
    }
    else
    {
        struct candy c = buffer[out];
        printf("consumer %d consumed %s\n", cCnt, &candyArr[out].type);
        out = (out + 1) % BUFFER_SIZE;
        candyBox[currentCandyInBox] = c;
        currentCandyInBox++;
    }

    sem_post(&semEmpty);
    pthread_exit(NULL);
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

void pChoice()
{
    int noProthread;
    printf("How many candy you want to produce: ");
    scanf("%d", &noProthread);
    for (int i = 0; i < noProthread; i++)
    {
        if (proController >= 0)
        {
            int selected = selectCandy();
            pthread_t *producers = (pthread_t *)malloc(candyTypes * sizeof(pthread_t));
            pthread_create(&producers[pCnt], NULL, candyProducer, (void *)i);
            printf("Producer %d: produced %s candy", i, &candyArr[selected].type);
            printf("Stored at %d of the Buffer\n", in);
        }
        else
        {
            printf("Buffer is FULL, please try again later");
        }
    }
}

void cChoice()
{
    printf("Maximum Candy in a box (Enter Size): ");
    scanf("%d", &boxSize);

    int numConsumer;
    printf("\nHow many candy you want to consume: ");
    scanf("%d", &numConsumer);
    for (int i = 0; i < numConsumer; i++)
    {
        if (conController > 0)
        {
            pthread_t *consumers = (pthread_t *)malloc(numBoxs * sizeof(pthread_t));
            pthread_create(&consumers[cCnt], NULL, candyConsumer, (void *)i);
            // printf("Consumer %d: consume ", cCnt);
            // printf("%s candy", &buffer[out].type);
            // printf(" from buffer %d\n", out);
        }

        else
        {
            printf("Buffer is empty. consumer cannot consume candy now.\n");
        }
    }
}

void whatInPack()
{
    if (flag == true)
    {
        for (int i = 0; i < boxSize; i++)
        {
            printf("%d %s", i, &candyBox[i].type);
        }
    }
    else
    {
        printf("No Box created yet.");
    }
}

void menu()
{
    printf("\n1. Producer\n2. Consumer\n3. Show Candy in Package\n4. Exit\n");
}

void showBuffer(){
    for(int i = 0; i < takeBufferSize; i++){
        if(buffer[i].type == NULL){
            break;
        }
        else{
            printf("At %d have candy %s\n" , i , &buffer[i].type);
        }
    }
}

int main()
{
    printf("Enter Assembly line size (BufferSize 1 - 1000): ");
    scanf("%d", &takeBufferSize);

    createCandy();
    proController = takeBufferSize;

    // initalize semaphore
    sem_init(&semEmpty, 0, BUFFER_SIZE);
    sem_init(&semFull, 0, 0);
    sem_init(&mutex_var, 0, 1);

    pthread_t producers[1000], consumers[1000];
    pthread_mutex_init(&mutex_var, NULL);

    int choice;
    printf("Welcome to Amherst Candy Factory\n");

    menu();
    printf("Enter your choice: ");
    do
    {
        menu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            pChoice();
            break;
        case 2:
            cChoice();
            scanf("%d", &choice);
            break;
        case 3:
            whatInPack();
            scanf("%d", &choice);
            break;
        case 4:
            showBuffer();
            break;
        case 5:
            exit(0);
        }
    } while (1);

    return 0;
}