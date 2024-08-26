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

#ifndef BINARY_SEMAPHORE_H
#define BINARY_SEMAPHORE_H

#include <pthread.h>

// Estructura del semáforo binario
typedef struct {
    pthread_mutex_t mutex;      // Mutex para proteger el acceso al semáforo
    pthread_cond_t conditional; // Variable de condición para sincronización
    int value;                  // Número entero que indica el estado del semáforo (0 para bloqueado, 1 para desbloqueado)
} binary_semaphore_t;

// Función que inicializa un semáforo binario con un valor inicial
void binary_semaphore_init(binary_semaphore_t *binary_semaphore, int value);
// Función que realiza la operación de espera en el semáforo binario, bloqueando si el valor es 0 hasta que se convierte en 1
void binary_semaphore_wait(binary_semaphore_t *binary_semaphore);
// Función que realiza la operación de señalización en el semáforo binario, cambiando su valor a 1 y despertando a un hilo en espera
void binary_semaphore_post(binary_semaphore_t *binary_semaphore);
// Función que destruye el semáforo binario, liberando los recursos utilizados para el mutex y la variable de condición
void binary_semaphore_destroy(binary_semaphore_t *binary_semaphore);

#endif // BINARY_SEMAPHORE_H