#include "rwlock.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READERS 5
#define NUMSTRINGS 3
#define BUFLENGTH 1024

char buffer[BUFLENGTH] = {0};
ReadWriteLock rwlock;

char *strings[NUMSTRINGS] = {
    "Help me Obiwan Kenobu, you're my only hope!",
    "When 900 years old you reach, look as good, you will not.",
    "I've got a bad feeling about this."};

void *readerthreadfunc(void *arg)
{
    long threadId = (long)arg;

    while (true)
    {
        rwlock_rdlock(&rwlock);
        printf("%ld: %s\n", threadId, buffer);
        usleep(250000);
        rwlock_unlock(&rwlock);
        usleep(500000);
    }

    return NULL;
}

void slow_copy(char *dest, char *src, int length)
{
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
        if (dest[i] == 0)
            break;
        if (i + 1 == length)
        {
            dest[i] = 0;
            break;
        }
        usleep(50000);
    }
}

int main(void)
{
    pthread_t readers[READERS];

    rwlock_init(&rwlock);

    for (long i = 0; i < READERS; i++)
    {
        pthread_create(&readers[i], NULL, readerthreadfunc, (void *)i);
    }

    int i = 0;
    while (true)
    {
        rwlock_wrlock(&rwlock);
        slow_copy(buffer, strings[i], BUFLENGTH);
        rwlock_unlock(&rwlock);
        i = (i + 1) % NUMSTRINGS;
        sleep(2);
    }

    // This line will never be reached, but you would normally clean up here
    rwlock_destroy(&rwlock);

    return 0;
}
