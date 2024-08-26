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

#include "mutex.h"

void mutex_init(mutex_t *parameter_mutex) {
    /*
        Función que inicializa un mutex
 
        Entradas:
        - parameter_mutex: Puntero a la estructura del mutex que contiene el mutex a inicializar

        Salidas:
        - Ninguna

        Restricciones:
        - La variable parameter_mutex no debe ser NULL y debe apuntar a un área de memoria válida
    */
    pthread_mutex_init(&parameter_mutex->mutex, NULL);
}

void mutex_lock(mutex_t *parameter_mutex) {
    /*
        Función que bloquea un mutex para sincronización entre hilos

        Entradas:
        - parameter_mutex: Puntero a la estructura del mutex que contiene el mutex a bloquear

        Salidas:
        - Ninguna

        Restricciones:
        - La variable parameter_mutex no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el mutex ha sido previamente inicializado con mutex_init
    */
    pthread_mutex_lock(&parameter_mutex->mutex);
}

void mutex_unlock(mutex_t *parameter_mutex) {
    /*
        Función que desbloquea un mutex, permitiendo que otros hilos continúen su ejecución

        Entradas:
        - parameter_mutex: Puntero a la estructura del mutex que contiene el mutex a desbloquear

        Salidas:
        - Ninguna

        Restricciones:
        - La variable parameter_mutex no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el mutex ha sido previamente inicializado con mutex_init y bloqueado por el hilo actual
    */
    pthread_mutex_unlock(&parameter_mutex->mutex);
}

void mutex_destroy(mutex_t *parameter_mutex) {
    /*
        Función que destruye un mutex, liberando los recursos asociados

        Entradas:
        - parameter_mutex: Puntero a la estructura del mutex que contiene el mutex a destruir

        Salidas:
        - Ninguna

        Restricciones:
        - La variable parameter_mutex no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el mutex ha sido previamente inicializado con mutex_init
    */
    pthread_mutex_destroy(&parameter_mutex->mutex);
}