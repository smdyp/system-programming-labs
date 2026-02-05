#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int arr[SIZE] = {0};
pthread_mutex_t mutex;

void* worker(void* arg) {
    int i = *(int*)arg;
    free(arg);
    
    pthread_mutex_lock(&mutex);
    
    arr[i] = i * 10;
    printf("Поток %d записал %d в arr[%d]\n", i, arr[i], i);
    
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main() {
    pthread_t threads[SIZE];
    
    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < SIZE; i++) {
        int* index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, worker, index);
    }
    
    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\nФинальный массив: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    pthread_mutex_destroy(&mutex);
    
    return 0;
}