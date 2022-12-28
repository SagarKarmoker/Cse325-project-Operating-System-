#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void *rollDice(){
    int getNum = (rand() % 6) + 1; 
    // dynamically allocated memory
    // beacuse getNum cannot accessed by main function
    int *result = malloc(sizeof(int));
    *result = getNum; 

    printf("Dice result: %p\n", result); // %p beacuse result is a pointer
    printf("Dice result: %d\n", *result); 
    
    return (void*) result;
}

int main(){
    int *res;
    srand(time(NULL));

    pthread_t thread;

    if(pthread_create(&thread, NULL, &rollDice, NULL) != 0){
        printf("Thread creation failed");
    }
    if(pthread_join(thread, (void**) &res) != 0){
        printf("Thread join failed");
    }
    
    printf("\nUnder the main function:\n");
    printf("Parent result: %p\n", res);
    printf("Final result: %d\n", *res);
    free(res);

    return 0;
}