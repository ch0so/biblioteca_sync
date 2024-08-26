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

#include "binary_semaphore.h"

void binary_semaphore_init(binary_semaphore_t *binary_semaphore, int value) {
    /*
        Función que inicializa un semáforo binario con un valor inicial

        Entradas:
        - binary_semaphore: Puntero a la estructura del semáforo binario que contiene un mutex y una variable de condición
        - value: Número entero que indica el valor inicial del semáforo; debe ser 0 o 1

        Salidas:
        - Ninguna

        Restricciones:
        - La variable binary_semaphore no debe ser NULL y debe apuntar a un área de memoria válida
        - La variable value debe ser un número entero que solo puede ser 0 o 1

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_init(&binary_semaphore->mutex, NULL);
    pthread_cond_init(&binary_semaphore->conditional, NULL);
    binary_semaphore->value = value;
}

void binary_semaphore_wait(binary_semaphore_t *binary_semaphore) {
    /*
        Función que realiza la operación de espera en el semáforo binario, bloqueando si el valor es 0 hasta que se convierte en 1

        Entradas:
        - binary_semaphore: Puntero a la estructura del semáforo binario que contiene un mutex y una variable de condición

        Salidas:
        - Ninguna

        Restricciones:
        - La variable binary_semaphore no debe ser NULL y debe apuntar a un área de memoria válida

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_lock(&binary_semaphore->mutex);
    while (binary_semaphore->value == 0) {
        pthread_cond_wait(&binary_semaphore->conditional, &binary_semaphore->mutex);
    }
    binary_semaphore->value = 0;
    pthread_mutex_unlock(&binary_semaphore->mutex);
}

void binary_semaphore_post(binary_semaphore_t *binary_semaphore) {
    /*
        Función que realiza la operación de señalización en el semáforo binario, cambiando su valor a 1 y despertando a un hilo en espera

        Entradas:
        - binary_semaphore: Puntero a la estructura del semáforo binario que contiene un mutex y una variable de condición

        Salidas:
        - Ninguna

        Restricciones:
        - La variable binary_semaphore no debe ser NULL y debe apuntar a un área de memoria válida

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_lock(&binary_semaphore->mutex);
    binary_semaphore->value = 1;
    pthread_cond_signal(&binary_semaphore->conditional);
    pthread_mutex_unlock(&binary_semaphore->mutex);
}

void binary_semaphore_destroy(binary_semaphore_t *binary_semaphore) {
    /*
        Función que destruye el semáforo binario, liberando los recursos utilizados para el mutex y la variable de condición

        Entradas:
        - binary_semaphore: Puntero a la estructura del semáforo binario que contiene un mutex y una variable de condición

        Salidas:
        - Ninguna

        Restricciones:
        - La variable binary_semaphore no debe ser NULL y debe apuntar a un área de memoria válida

        Referencias bibliográficas:
        - GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
        - GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
        - IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
    */
    pthread_mutex_destroy(&binary_semaphore->mutex);
    pthread_cond_destroy(&binary_semaphore->conditional);
}