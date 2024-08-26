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

#include "counting_semaphore.h"

void counting_semaphore_init(counting_semaphore_t *counting_semaphore, int value) {
    /*
        Función que inicializa un semáforo de conteo con un valor inicial

        Entradas:
        - counting_semaphore: Puntero a la estructura del semáforo de conteo que contiene un mutex, una variable de condición, y un contador
        - value: Número entero que indica el valor inicial del semáforo; representa el número de recursos disponibles

        Salidas:
        - Ninguna

        Restricciones:
        - La variable counting_semaphore no debe ser NULL y debe apuntar a un área de memoria válida
        - La variable value debe ser un número entero no negativo

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_init(&counting_semaphore->mutex, NULL);
    pthread_cond_init(&counting_semaphore->conditional, NULL);
    counting_semaphore->value = value;
}

void counting_semaphore_wait(counting_semaphore_t *counting_semaphore) {
    /*
        Función que decrementa el contador del semáforo de conteo, bloqueando si no hay recursos disponibles

        Entradas:
        - counting_semaphore: Puntero a la estructura del semáforo de conteo que contiene un mutex, una variable de condición, y un contador

        Salidas:
        - Ninguna

        Restricciones:
        - La variable counting_semaphore no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el semáforo ha sido previamente inicializado con counting_semaphore_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_lock(&counting_semaphore->mutex);
    while (counting_semaphore->value <= 0) {
        pthread_cond_wait(&counting_semaphore->conditional, &counting_semaphore->mutex);
    }
    counting_semaphore->value--;
    pthread_mutex_unlock(&counting_semaphore->mutex);
}

void counting_semaphore_post(counting_semaphore_t *counting_semaphore) {
    /*
        Función que incrementa el contador del semáforo de conteo, señalando a cualquier hilo bloqueado que puede continuar

        Entradas:
        - counting_semaphore: Puntero a la estructura del semáforo de conteo que contiene un mutex, una variable de condición, y un contador

        Salidas:
        - Ninguna

        Restricciones:
        - La variable counting_semaphore no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el semáforo ha sido previamente inicializado con counting_semaphore_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_lock(&counting_semaphore->mutex);
    counting_semaphore->value++;
    pthread_cond_signal(&counting_semaphore->conditional);
    pthread_mutex_unlock(&counting_semaphore->mutex);
}

void counting_semaphore_destroy(counting_semaphore_t *counting_semaphore) {
    /*
        Función que destruye el semáforo de conteo, liberando los recursos asociados

        Entradas:
        - counting_semaphore: Puntero a la estructura del semáforo de conteo que contiene un mutex, una variable de condición, y un contador

        Salidas:
        - Ninguna

        Restricciones:
        - La variable counting_semaphore no debe ser NULL y debe apuntar a un área de memoria válida
        - Se asume que el semáforo ha sido previamente inicializado con counting_semaphore_init

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_destroy(&counting_semaphore->mutex);
    pthread_cond_destroy(&counting_semaphore->conditional);
}