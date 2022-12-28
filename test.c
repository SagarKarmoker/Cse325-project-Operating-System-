#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define TOT_THREAD 4

int count = 0;
int in = 0;
int cur = 10;
int buffer[100];
void *counter(){
    // for(int i = 0; i < 100; i++){
    //     count++;
    // }
    buffer[in] = cur+1;
    in = (in + 1) % 10;
}

int main()
{
    pthread_t th[TOT_THREAD];

    for(int i =0; i < 10;i++){
        buffer[i] = -1;
    }

    for(int i = 1; i <= TOT_THREAD; i++){
        pthread_t *pro = (pthread_t *)malloc(sizeof(pthread_t*));
        //if(pthread_create(th + i, NULL, &counter, NULL) != 0){
        if(pthread_create(th[pro], NULL, &counter, NULL) != 0){
            printf("Failed to create a thread");
        }
        else{
            printf("Thread %d has started\n", i);

        }
        
        if(pthread_join(th[i], NULL) != 0){
            printf("Failed to join thread to main");
        }
        else{
            printf("Thread %d has finished execution\n", i);
        }

        printf("Thread %d counted: %d\n\n", i, count);
    }

    for(int i =0; i < 10; i++){
        printf("%d ", buffer[i]);
    }
    
    printf("Final Total Number counted: %d\n", count);

    return 0;
}