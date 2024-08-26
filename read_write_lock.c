/*
Código creado por:
    Ávila Ramírez Paublo Alexander
    pavila@estudiantec.cr
    2022035584
    
    
	Reyes Rodríguez	Ricardo Andrés
    rireyes@estudiantec.cr
    2022101681

    
	Zúñiga Campos Omar Jesús
    omzuniga@estudiantec.cr
    2022019053

Referencias bibliográficas:
- GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
- IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
- IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
*/

#include "read_write_lock.h"

void read_write_lock_init(read_write_lock_t *read_write_lock) {
    /*
        Función que inicializa un candado de lectura/escritura

        Entradas:
        - read_write_lock: Puntero a la estructura del candado de lectura/escritura que contiene un mutex, dos variables de condición y contadores

        Salidas:
        - Ninguna

        Restricciones:
        - La variable read_write_lock no debe ser NULL y debe apuntar a un área de memoria válida

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
        - IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
    */
    pthread_mutex_init(&read_write_lock->mutex, NULL);
    pthread_cond_init(&read_write_lock->read, NULL);
    pthread_cond_init(&read_write_lock->write, NULL);
    read_write_lock->readers = 0;
    read_write_lock->writers = 0;
    read_write_lock->writing_flag = false;
}

void read_write_lock_read_lock(read_write_lock_t *read_write_lock) {
    /*
        Función que bloquea el candado de lectura, permitiendo que múltiples hilos lean simultáneamente si no hay escritores activos

        Entradas:
        - read_write_lock: Puntero a la estructura del candado de lectura/escritura que contiene un mutex, dos variables de condición y contadores

        Salidas:
        - Ninguna

        Restricciones:
        - La variable read_write_lock no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el candado ha sido previamente inicializado con read_write_lock_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
        - IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
    */
    pthread_mutex_lock(&read_write_lock->mutex);

    while (read_write_lock->writing_flag) {
        pthread_cond_wait(&read_write_lock->read, &read_write_lock->mutex);
    }

    read_write_lock->readers++;
    pthread_mutex_unlock(&read_write_lock->mutex);
}

void read_write_lock_write_lock(read_write_lock_t *read_write_lock) {
    /*
        Función que bloquea el candado de escritura, garantizando exclusividad para un único hilo escritor

        Entradas:
        - read_write_lock: Puntero a la estructura del candado de lectura/escritura que contiene un mutex, dos variables de condición y contadores

        Salidas:
        - Ninguna

        Restricciones:
        - La variable read_write_lock no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el candado ha sido previamente inicializado con read_write_lock_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
        - IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
    */
    pthread_mutex_lock(&read_write_lock->mutex);
    read_write_lock->writers++;

    while (read_write_lock->readers > 0) {
        pthread_cond_wait(&read_write_lock->write, &read_write_lock->mutex);
    }

    read_write_lock->writers--;
    read_write_lock->writing_flag = true;
    pthread_mutex_unlock(&read_write_lock->mutex);
}

void read_write_lock_unlock(read_write_lock_t *read_write_lock) {
    /*
        Función que libera el candado de lectura o escritura, permitiendo que otros hilos continúen

        Entradas:
        - read_write_lock: Puntero a la estructura del candado de lectura/escritura que contiene un mutex, dos variables de condición y contadores

        Salidas:
        - Ninguna

        Restricciones:
        - La variable read_write_lock no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el candado ha sido previamente inicializado con read_write_lock_init y bloqueado por el hilo actual

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
        - IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
    */
    pthread_mutex_lock(&read_write_lock->mutex);
    if (read_write_lock->writing_flag) {
        read_write_lock->writing_flag = false;
        pthread_cond_broadcast(&read_write_lock->read);
        pthread_cond_signal(&read_write_lock->write);
    } else {
        read_write_lock->readers--;
        if ((read_write_lock->readers == 0) && (read_write_lock->writers > 0)) {
            pthread_cond_signal(&read_write_lock->write);
        }
    }
    pthread_mutex_unlock(&read_write_lock->mutex);
}

void read_write_lock_destroy(read_write_lock_t *read_write_lock) {
    /*
        Función que destruye el candado de lectura/escritura, liberando los recursos asociados

        Entradas:
        - read_write_lock: Puntero a la estructura del candado de lectura/escritura que contiene un mutex, dos variables de condición y contadores

        Salidas:
        - Ninguna

        Restricciones:
        - La variable read_write_lock no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el candado ha sido previamente inicializado con read_write_lock_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
        - IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
    */
    pthread_mutex_destroy(&read_write_lock->mutex);
    pthread_cond_destroy(&read_write_lock->read);
    pthread_cond_destroy(&read_write_lock->write);
}