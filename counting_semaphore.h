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
- GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
- IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
*/

#ifndef COUNTING_SEMAPHORE_H
#define COUNTING_SEMAPHORE_H

#include <pthread.h>

// Estructura del semáforo de conteo
typedef struct {
    pthread_mutex_t mutex;      // Mutex para proteger el acceso al semáforo de conteo
    pthread_cond_t conditional; // Variable de condición para sincronización entre hilos
    int value;                  // Número entero que indica el contador del semáforo
} counting_semaphore_t;

// Función que inicializa un semáforo de conteo con un valor inicial
void counting_semaphore_init(counting_semaphore_t *counting_semaphore, int value);
// Función que decrementa el contador del semáforo de conteo, bloqueando si no hay recursos disponibles
void counting_semaphore_wait(counting_semaphore_t *counting_semaphore);
// Función que incrementa el contador del semáforo de conteo, señalando a cualquier hilo bloqueado que puede continuar
void counting_semaphore_post(counting_semaphore_t *counting_semaphore);
// Función que destruye el semáforo de conteo, liberando los recursos asociados
void counting_semaphore_destroy(counting_semaphore_t *counting_semaphore);

#endif // COUNTING_SEMAPHORE_H