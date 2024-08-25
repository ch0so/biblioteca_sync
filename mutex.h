// mutex.h

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
} mutex_t;

void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);
void mutex_destroy(mutex_t *m);

#endif // MUTEX_H