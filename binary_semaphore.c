#include "binary_semaphore.h"

void binary_semaphore_init(binary_semaphore_t *sem, int value) {
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
    sem->value = value;
}

void binary_semaphore_wait(binary_semaphore_t *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value = 0;
    pthread_mutex_unlock(&sem->mutex);
}

void binary_semaphore_post(binary_semaphore_t *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value = 1;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

void binary_semaphore_destroy(binary_semaphore_t *sem) {
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);
}