#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Definiciones y variables globales para diferentes ejemplos
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define HILOS 10
#define READERS 5
#define NUMSTRINGS 3
#define BUFLENGTH 1024
#define NUM_THREADS 4

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;
int ids[HILOS];

binary_semaphore_t empty;
binary_semaphore_t full;
counting_semaphore_t sem;
barrier_t barr;
ReadWriteLock rwlock;
mutex_t mtx;

char shared_buffer[BUFLENGTH] = {0};
char *strings[NUMSTRINGS] = {
    "Help me Obiwan Kenobi, you're my only hope!",
    "When 900 years old you reach, look as good, you will not.",
    "I've got a bad feeling about this."};

// Funciones para el ejemplo de semáforo binario: productor-consumidor
void* producer_binary(void* arg) {
    int id = *((int*) arg);
    while (1) {
        int item = rand() % 100;
        binary_semaphore_wait(&empty);
        buffer[in] = item;
        printf("Producer %d: produced %d, count = %d\n", id, item, ++count);
        in = (in + 1) % BUFFER_SIZE;
        binary_semaphore_post(&full);
        sleep(1);
    }
    return NULL;
}

void* consumer_binary(void* arg) {
    int id = *((int*) arg);
    while (1) {
        binary_semaphore_wait(&full);
        int item = buffer[out];
        printf("Consumer %d: consumed %d, count = %d\n", id, item, --count);
        out = (out + 1) % BUFFER_SIZE;
        binary_semaphore_post(&empty);
        sleep(1);
    }
    return NULL;
}

// Funciones para el ejemplo de semáforo de conteo: contador simple
int shared_counter = 0;

void* increment_counter(void* arg) {
    int id = *((int*) arg);
    for (int i = 0; i < 10; i++) {
        counting_semaphore_wait(&sem);
        shared_counter++;
        printf("Thread %d incremented counter to %d\n", id, shared_counter);
        counting_semaphore_post(&sem);
        sleep(1);
    }
    return NULL;
}

void* decrement_counter(void* arg) {
    int id = *((int*) arg);
    for (int i = 0; i < 10; i++) {
        counting_semaphore_wait(&sem);
        shared_counter--;
        printf("Thread %d decremented counter to %d\n", id, shared_counter);
        counting_semaphore_post(&sem);
        sleep(1);
    }
    return NULL;
}

// Funciones para el ejemplo de barrera
void* func_ej(void* arg) {
    int id = *((int*) arg);
    printf("Thread %d reached the barrier\n", id);
    barrier_wait(&barr);
    printf("Thread %d passed the barrier\n", id);
    return NULL;
}

// Funciones para el ejemplo de Read/Write Lock
void* readerthreadfunc(void* arg) {
    long threadId = (long)arg;
    while (1) {
        rwlock_rdlock(&rwlock);
        printf("%ld: %s\n", threadId, shared_buffer);
        usleep(250000);
        rwlock_unlock(&rwlock);
        usleep(500000);
    }
    return NULL;
}

void slow_copy(char *dest, char *src, int length) {
    for (int i = 0; i < length; i++) {
        dest[i] = src[i];
        if (dest[i] == 0) break;
        if (i + 1 == length) {
            dest[i] = 0;
            break;
        }
        usleep(50000);
    }
}

// Funciones para el ejemplo de mutex: Lectores-Escritores
void* writer_func(void* arg) {
    int id = *((int*) arg);
    while (1) {
        mutex_lock(&mtx);
        snprintf(shared_buffer, BUFLENGTH, "Writer %d was here!", id);
        printf("Writer %d wrote to the buffer.\n", id);
        mutex_unlock(&mtx);
        sleep(2);
    }
    return NULL;
}

void* reader_func(void* arg) {
    int id = *((int*) arg);
    while (1) {
        mutex_lock(&mtx);
        printf("Reader %d read: %s\n", id, shared_buffer);
        mutex_unlock(&mtx);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <binary|counting|barrier|rwlock|mutex>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "binary") == 0) {
        pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

        binary_semaphore_init(&empty, BUFFER_SIZE);
        binary_semaphore_init(&full, 0);

        for (int i = 0; i < NUM_PRODUCERS; i++) {
            pthread_create(&producers[i], NULL, producer_binary, &ids[i]);
        }

        for (int i = 0; i < NUM_CONSUMERS; i++) {
            pthread_create(&consumers[i], NULL, consumer_binary, &ids[i]);
        }

        for (int i = 0; i < NUM_PRODUCERS; i++) {
            pthread_join(producers[i], NULL);
        }

        for (int i = 0; i < NUM_CONSUMERS; i++) {
            pthread_join(consumers[i], NULL);
        }

        binary_semaphore_destroy(&empty);
        binary_semaphore_destroy(&full);
    } else if (strcmp(argv[1], "counting") == 0) {
        pthread_t threads[NUM_THREADS];

        counting_semaphore_init(&sem, 1);

        for (int i = 0; i < NUM_THREADS; i++) {
            if (i % 2 == 0) {
                pthread_create(&threads[i], NULL, increment_counter, &ids[i]);
            } else {
                pthread_create(&threads[i], NULL, decrement_counter, &ids[i]);
            }
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        counting_semaphore_destroy(&sem);
    } else if (strcmp(argv[1], "barrier") == 0) {
        pthread_t hilos[HILOS];

        barrier_init(&barr, HILOS);

        for (int i = 0; i < HILOS; i++) {
            ids[i] = i;
            pthread_create(&hilos[i], NULL, func_ej, &ids[i]);
        }

        for (int i = 0; i < HILOS; i++) {
            pthread_join(hilos[i], NULL);
        }

        barrier_destroy(&barr);
    } else if (strcmp(argv[1], "rwlock") == 0) {
        pthread_t readers[READERS];

        rwlock_init(&rwlock);

        for (long i = 0; i < READERS; i++) {
            pthread_create(&readers[i], NULL, readerthreadfunc, (void *)i);
        }

        int i = 0;
        while (1) {
            rwlock_wrlock(&rwlock);
            slow_copy(shared_buffer, strings[i], BUFLENGTH);
            rwlock_unlock(&rwlock);
            i = (i + 1) % NUMSTRINGS;
            sleep(2);
        }

        rwlock_destroy(&rwlock);
    } else if (strcmp(argv[1], "mutex") == 0) {
        pthread_t writers[NUM_THREADS], readers[NUM_THREADS];

        mutex_init(&mtx);

        for (int i = 0; i < NUM_THREADS; i++) {
            ids[i] = i;
            if (i % 2 == 0) {
                pthread_create(&writers[i], NULL, writer_func, &ids[i]);
            } else {
                pthread_create(&readers[i], NULL, reader_func, &ids[i]);
            }
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            if (i % 2 == 0) {
                pthread_join(writers[i], NULL);
            } else {
                pthread_join(readers[i], NULL);
            }
        }

        mutex_destroy(&mtx);
    } else {
        printf("Invalid argument. Usage: %s <binary|counting|barrier|rwlock|mutex>\n", argv[0]);
        return 1;
    }

    return 0;
}