#ifndef BINARY_SEMAPHORE_H
#define BINARY_SEMAPHORE_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} binary_semaphore_t;

void binary_semaphore_init(binary_semaphore_t *sem, int value);
void binary_semaphore_wait(binary_semaphore_t *sem);
void binary_semaphore_post(binary_semaphore_t *sem);
void binary_semaphore_destroy(binary_semaphore_t *sem);

#endif // BINARY_SEMAPHORE_H