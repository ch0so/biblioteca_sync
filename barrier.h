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
- Desai, J. (2020, 11 junio). Barrier Synchronization in Threads - Jay Desai - Medium [Sincronización de Barreras en Hilos]. Medium. https://medium.com/@jaydesai36/barrier-synchronization-in-threads-3c56f947047
- GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
*/

#ifndef BARRIER_H
#define BARRIER_H

#include <pthread.h>

// Estructura de la barrera
typedef struct {
    pthread_mutex_t mutex; // Mutex para proteger el acceso a la barrera
    pthread_cond_t conditional;   // Variable de condición para sincronización
    int count;             // Número de hilos que deben sincronizarse
    int wait;              // Número actual de hilos que están esperando
} barrier_t;

// Función que inicializa la barrera con el número de hilos que deben sincronizarse
void barrier_init(barrier_t *barrier, int count);

// Función que espera a que todos los hilos alcancen la barrera
void barrier_wait(barrier_t *barrier);

// Función que destruye la barrera, liberando los recursos utilizados
void barrier_destroy(barrier_t *barrier);

#endif // BARRIER_H
