#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;
int count = 0;

void* increment(void* arg) {
    pthread_mutex_lock(&lock);
    count++;
    printf("Thread %d incremented count to %d\n", *(int*)arg, count);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, increment, &thread_ids[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}