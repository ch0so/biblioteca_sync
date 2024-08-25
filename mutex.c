#include "mutex.h"

void mutex_init(mutex_t *m) {
    pthread_mutex_init(&m->mutex, NULL);
}

void mutex_lock(mutex_t *m) {
    pthread_mutex_lock(&m->mutex);
}

void mutex_unlock(mutex_t *m) {
    pthread_mutex_unlock(&m->mutex);
}

void mutex_destroy(mutex_t *m) {
    pthread_mutex_destroy(&m->mutex);
}