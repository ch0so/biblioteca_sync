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


#ifndef READ_WIRTE_LOCK_H
#define READ_WIRTE_LOCK_H

#include <pthread.h>
#include <stdbool.h>

// Estructura del candado de lectura/escritura
typedef struct {
    pthread_mutex_t mutex;       // Mutex para proteger el acceso al candado de lectura/escritura
    pthread_cond_t read;         // Variable de condición para sincronización de los lectores
    pthread_cond_t write;        // Variable de condición para sincronización de los escritores
    int readers;                 // Número entero que indica la cantidad actual de lectores
    int writers;                 // Número entero que indica la cantidad actual de escritores en espera
    int writing_flag;            // Indicador entero que señala si un escritor está escribiendo actualmente
} read_write_lock_t;

// Función que inicializa un candado de lectura/escritura
void read_write_lock_init(read_write_lock_t *read_write_lock);
// Función que bloquea el candado de lectura, permitiendo que múltiples hilos lean simultáneamente si no hay escritores activos
void read_write_lock_read_lock(read_write_lock_t *read_write_lock);
// Función que bloquea el candado de escritura, garantizando exclusividad para un único hilo escritor
void read_write_lock_write_lock(read_write_lock_t *read_write_lock);
// Función que libera el candado de lectura o escritura, permitiendo que otros hilos continúen
void read_write_lock_unlock(read_write_lock_t *read_write_lock);
// Función que destruye el candado de lectura/escritura, liberando los recursos asociados
void read_write_lock_destroy(read_write_lock_t *read_write_lock);

#endif // READ_WIRTE_LOCK_H