#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>
#include <stdbool.h>

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t read;
    pthread_cond_t write;
    int readers;
    int writers;
    int writingFlag;
} ReadWriteLock;

void rwlock_init(ReadWriteLock *rwlock);
void rwlock_rdlock(ReadWriteLock *rwlock);
void rwlock_wrlock(ReadWriteLock *rwlock);
void rwlock_unlock(ReadWriteLock *rwlock);
void rwlock_destroy(ReadWriteLock *rwlock);

#endif // RWLOCK_H