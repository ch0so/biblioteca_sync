#ifndef COUNTING_SEMAPHORE_H
#define COUNTING_SEMAPHORE_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} counting_semaphore_t;

void counting_semaphore_init(counting_semaphore_t *sem, int value);
void counting_semaphore_wait(counting_semaphore_t *sem);
void counting_semaphore_post(counting_semaphore_t *sem);
void counting_semaphore_destroy(counting_semaphore_t *sem);

#endif // COUNTING_SEMAPHORE_H