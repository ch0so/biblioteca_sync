#include "rwlock.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_STRINGS 3
#define BUFFER_LENGTH 1024

char buffer[BUFFER_LENGTH] = {0};
ReadWriteLock rwlock;

char *messages[NUM_STRINGS] = {
    "Cadena de texto #1",
    "Esta es la segunda cadena de texto",
    "Estamos en la ultima cadena de texto del ejemplo"};

void *readerThread(void *arg)
{
    long threadId = (long)arg;

    while (1)
    {
        rwlock_rdlock(&rwlock);
        printf("Reader %ld: %s\n", threadId, buffer);
        usleep(250000); // Tiempo para simular lectura
        rwlock_unlock(&rwlock);
        usleep(500000); // Tiempo antes de leer de nuevo
    }

    return NULL;
}

void slowMessageCopy(char *dest, const char *src, int length)
{
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
        if (dest[i] == '\0')
            break;
        usleep(50000); // Tiempo entre cada carácter copiado
    }
}

int main(void)
{
    pthread_t readers[NUM_READERS];

    rwlock_init(&rwlock);

    // Crea hilos lectores
    for (long i = 0; i < NUM_READERS; i++)
    {
        pthread_create(&readers[i], NULL, readerThread, (void *)i);
    }

    int i = 0;
    while (1)
    {
        rwlock_wrlock(&rwlock);
        slowMessageCopy(buffer, messages[i], BUFFER_LENGTH);
        rwlock_unlock(&rwlock);
        i = (i + 1) % NUM_STRINGS;
        sleep(2);
    }

    // Normalmente se limpiarían los recursos aquí (Nunca se alcanza esta línea)
    rwlock_destroy(&rwlock);

    return 0;
}