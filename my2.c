#include <stdio.h>
#include <pthread.h>
#define TOT_THREAD 4

int count = 0;

void *counter(){
    for(int i = 0; i < 100; i++){
        count++;
    }
}

int main()
{
    pthread_t th[TOT_THREAD];

    for(int i = 1; i <= TOT_THREAD; i++){
        if(pthread_create(th + i, NULL, &counter, NULL) != 0){
            printf("Failed to create a thread");
        }
        else{
            printf("Thread %d has started\n", i);

        }
        
        // if(pthread_join(th[i], NULL) != 0){
        //     printf("Failed to join thread to main");
        // }
        // else{
        //     printf("Thread %d has finished execution\n", i);
        // }

        printf("Thread %d counted: %d\n\n", i, count);
    }
    
    printf("Final Total Number counted: %d\n", count);

    return 0;
}