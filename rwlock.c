#include "rwlock.h"

/*
 * Inicializa la estructura ReadWriteLock.
 * Esta función configura el mutex y las variables de condición, y
 * también inicializa los contadores de lectores y escritores, así como la bandera de escritura.
 *
 * @param rwlock: Un puntero a la estructura ReadWriteLock que se va a inicializar.
 */
void rwlock_init(ReadWriteLock *rwlock)
{
    pthread_mutex_init(&rwlock->mutex, NULL);
    pthread_cond_init(&rwlock->read, NULL);
    pthread_cond_init(&rwlock->write, NULL);
    rwlock->readers = 0;
    rwlock->writers = 0;
    rwlock->writingFlag = false;
}

/*
 * Adquiere un bloqueo de lectura en ReadWriteLock.
 * Esta función bloquea el hilo actual hasta que pueda adquirir
 * el bloqueo de lectura de forma segura. Garantiza que ningún escritor esté escribiendo actualmente
 * e incrementa el contador de lectores.
 *
 * @param rwlock: Un puntero a la estructura ReadWriteLock para bloquear.
 */
void rwlock_rdlock(ReadWriteLock *rwlock)
{
    pthread_mutex_lock(&rwlock->mutex);

    while (rwlock->writingFlag)
    {
        pthread_cond_wait(&rwlock->read, &rwlock->mutex);
    }

    rwlock->readers++;
    pthread_mutex_unlock(&rwlock->mutex);
}

/*
 * Adquiere un bloqueo de escritura en ReadWriteLock.
 * Esta función bloquea el hilo actual hasta que pueda adquirir
 * el bloqueo de escritura de forma segura. Garantiza que ningún lector esté leyendo actualmente
 * y que no haya otro escritor activo.
 *
 * @param rwlock: Un puntero a la estructura ReadWriteLock para bloquear.
 */
void rwlock_wrlock(ReadWriteLock *rwlock)
{
    pthread_mutex_lock(&rwlock->mutex);
    rwlock->writers++;

    while (rwlock->readers > 0)
    {
        pthread_cond_wait(&rwlock->write, &rwlock->mutex);
    }

    rwlock->writers--;
    rwlock->writingFlag = true;
    pthread_mutex_unlock(&rwlock->mutex);
}

/*
 * Libera un bloqueo (ya sea de lectura o de escritura) en ReadWriteLock.
 * Esta función desbloquea ReadWriteLock y señala a los hilos que estén esperando.
 * Si el hilo actual tiene un bloqueo de escritura, restablece la bandera de escritura y
 * señala tanto a lectores como a escritores. Si el hilo actual tiene un bloqueo de lectura,
 * decrementa el contador de lectores y señala a un escritor en espera si no quedan lectores.
 *
 * @param rwlock: Un puntero a la estructura ReadWriteLock para desbloquear.
 */
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

/*
 * Destruye la estructura ReadWriteLock.
 * Esta función libera los recursos asociados con el mutex y las variables de condición.
 * Debe ser llamada cuando ya no se necesite el ReadWriteLock.
 *
 * @param rwlock: Un puntero a la estructura ReadWriteLock para destruir.
 */
void rwlock_destroy(ReadWriteLock *rwlock)
{
    pthread_mutex_destroy(&rwlock->mutex);
    pthread_cond_destroy(&rwlock->read);
    pthread_cond_destroy(&rwlock->write);
}
