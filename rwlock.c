#include "rwlock.h"

void rwlock_init(ReadWriteLock *rwlock)
{
    pthread_mutex_init(&rwlock->mutex, NULL);
    pthread_cond_init(&rwlock->read, NULL);
    pthread_cond_init(&rwlock->write, NULL);
    rwlock->readers = 0;
    rwlock->writers = 0;
    rwlock->writingFlag = false;
}

void rwlock_rdlock(ReadWriteLock *rwlock)
{
    pthread_mutex_lock(&rwlock->mutex);
    while (rwlock->writingFlag || (rwlock->writers > 0))
    {
        pthread_cond_wait(&rwlock->read, &rwlock->mutex);
    }
    rwlock->readers++;
    pthread_mutex_unlock(&rwlock->mutex);
}

void rwlock_wrlock(ReadWriteLock *rwlock)
{
    pthread_mutex_lock(&rwlock->mutex);
    rwlock->writers++;
    while (rwlock->writingFlag || (rwlock->readers > 0))
    {
        pthread_cond_wait(&rwlock->write, &rwlock->mutex);
    }
    rwlock->writers--;
    rwlock->writingFlag = true;
    pthread_mutex_unlock(&rwlock->mutex);
}

void rwlock_unlock(ReadWriteLock *rwlock)
{
    pthread_mutex_lock(&rwlock->mutex);
    if (rwlock->writingFlag)
    {
        rwlock->writingFlag = false;
        pthread_cond_broadcast(&rwlock->read);
        pthread_cond_signal(&rwlock->write);
    }
    else
    {
        rwlock->readers--;
        if ((rwlock->readers == 0) && (rwlock->writers > 0))
        {
            pthread_cond_signal(&rwlock->write);
        }
    }
    pthread_mutex_unlock(&rwlock->mutex);
}

void rwlock_destroy(ReadWriteLock *rwlock)
{
    pthread_mutex_destroy(&rwlock->mutex);
    pthread_cond_destroy(&rwlock->read);
    pthread_cond_destroy(&rwlock->write);
}
