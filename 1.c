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
}candy;

int main(){
    struct candy c;
    c.id = 1;
    c.type = 'Kit';
    
    printf("%d %s", c.id, c.type);

    return 0;
}