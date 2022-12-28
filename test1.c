#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struct to hold the data for each thread 
typedef struct {
    int threadID;
    char * message;
} thread_data;

// Thread Function
void * threadFunc(void * arg)
{
    thread_data * data = (thread_data *)arg; 

    printf("Thread %d says %s \n", data->threadID, data->message);

    free(data);  // free memory 
    pthread_exit(NULL);
}

int main()
{
    // Create array of threads 
    pthread_t threads[10];
    int ret;

    // Create 10 threads
    for(int i=0; i<10; i++)
    {
        // Allocate memory for thread data
        thread_data * data = (thread_data *)malloc(sizeof(thread_data));
        data->threadID = i;
        data->message = "Hello World";

        // Create the thread
        ret = pthread_create(&threads[i], NULL, threadFunc, data);
        if (ret != 0)
        {
            printf("Error creating thread\n");
            return -1;
        }
    }

    // Join threads
    for(int i=0; i<10; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}