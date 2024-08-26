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
*/

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

// Estructura del mutex
typedef struct {
    pthread_mutex_t mutex; // Mutex para sincronización entre hilos
} mutex_t;

// Función que inicializa un mutex
void mutex_init(mutex_t *parameter_mutex);
// Función que bloquea un mutex para evitar condiciones de carrera
void mutex_lock(mutex_t *parameter_mutex);
// Función que desbloquea un mutex, permitiendo a otros hilos acceder a la sección crítica
void mutex_unlock(mutex_t *parameter_mutex);
// Función que destruye un mutex, liberando los recursos asociados
void mutex_destroy(mutex_t *parameter_mutex);

#endif // MUTEX_H