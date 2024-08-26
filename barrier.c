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
#include "barrier.h"

void barrier_init(barrier_t *barrier, int count) {
    /*
        Función que inicializa la estructura de una barrera con el número de hilos que deben sincronizarse

        Entradas:
        - barrier: Puntero a la estructura de la barrera que contiene un mutex y una variable de condición
        - count: Número entero que indica la cantidad de hilos que deben alcanzar la barrera antes de que se liberen

        Salidas:
        - Ninguna

        Restricciones:
        - La variable barrier no debe ser NULL y debe apuntar a un área de memoria válida
        - La variable count debe ser un número entero mayor que 0
        
        Referencias bibliográficas:
        - Desai, J. (2020, 11 junio). Barrier Synchronization in Threads - Jay Desai - Medium [Sincronización de Barreras en Hilos]. Medium. https://medium.com/@jaydesai36/barrier-synchronization-in-threads-3c56f947047
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
    */
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->conditional, NULL);
    barrier->count = count;
    barrier->wait = 0;
}

void barrier_wait(barrier_t *barrier) {
    /*
        Función que espera a que todos los hilos alcancen la barrera Si el número de hilos que ha alcanzado
        la barrera es igual al número especificado en la inicialización, se desbloquean todos los hilos

        Entradas:
        - barrier: Puntero a la estructura de la barrera que contiene un mutex y una variable de condición

        Salidas:
        - Ninguna

        Restricciones:
        - La variable barrier no debe ser NULL y debe apuntar a un área de memoria válida
        
        Referencias bibliográficas:
        - Desai, J. (2020, 11 junio). Barrier Synchronization in Threads - Jay Desai - Medium [Sincronización de Barreras en Hilos]. Medium. https://medium.com/@jaydesai36/barrier-synchronization-in-threads-3c56f947047
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
    */
    pthread_mutex_lock(&barrier->mutex);
    barrier->wait++;
    if (barrier->wait == barrier->count) {
        pthread_cond_broadcast(&barrier->conditional);
        barrier->wait = 0;
    } else {
        pthread_cond_wait(&barrier->conditional, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
}

void barrier_destroy(barrier_t *barrier) {
    /*
        Función que destruye la barrera, liberando los recursos utilizados para el mutex y la variable de condición

        Entradas:
        - barrier: Puntero a la estructura de la barrera que contiene un mutex y una variable de condición

        Salidas:
        - Ninguna

        Restricciones:
        - La variable barrier no debe ser NULL y debe apuntar a un área de memoria válida

        Referencias bibliográficas:
        - Desai, J. (2020, 11 junio). Barrier Synchronization in Threads - Jay Desai - Medium [Sincronización de Barreras en Hilos]. Medium. https://medium.com/@jaydesai36/barrier-synchronization-in-threads-3c56f947047
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
    */
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->conditional);
}