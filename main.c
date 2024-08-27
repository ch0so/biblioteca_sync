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
- GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
- IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
- IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "barrier.h"
#include "binary_semaphore.h"
#include "counting_semaphore.h"
#include "mutex.h"
#include "read_write_lock.h"

// Definiciones y variables globales para diferentes ejemplos
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 4
#define NUM_CONSUMERS 4
#define NUM_ITEMS 20
#define THREADS 10
#define NUM_READERS 5
#define NUM_STRINGS 4
#define BUFFER_LENGTH 1024
#define NUM_THREADS 10
#define INITIAL_COUNTER 20
#define INCREMENT_COUNT 5
#define DECREMENT_COUNT 5
#define STOP_VALUE 20
#define NUM_PERSONS 5 

int count = 0;
int in = 0;
int out = 0;
int current_message_index = 0;
int finished_writing = 0;
int shared_counter = INITIAL_COUNTER;
int produced_items = 0;
int consumed_items = 0;

binary_semaphore_t empty;
binary_semaphore_t full;
counting_semaphore_t counting_semaphore;
barrier_t barrier;
read_write_lock_t read_write_lock;
mutex_t mutex;

int ids[THREADS];
int buffer[BUFFER_SIZE];
char shared_buffer[BUFFER_LENGTH] = {0};
char *messages[NUM_STRINGS] = {
    "Cadena de texto #1",
    "Segunda cadena de texto",
    "Ahora es la tercera cadena de texto",
    "Cadena de texto final del ejemplo",
};

// Funciones para el ejemplo de semáforo binario: productor-consumidor
void* producer_binary(void* arg) {
    /*
        Función que simula un productor en el ejemplo de semáforo binario

        Entradas:
        - arg: Puntero a un entero que representa el identificador del productor

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    int id = *((int*) arg);
    for (int i = 0; i < NUM_ITEMS / NUM_PRODUCERS; i++) {
        int item = rand() % 100;
        binary_semaphore_wait(&empty);
        buffer[in] = item;
        printf("Producer %d: produced %d\n", id, item);
        in = (in + 1) % BUFFER_SIZE;
        produced_items++;
        binary_semaphore_post(&full);

        if (produced_items >= NUM_ITEMS) {
            break;
        }
        sleep(1);
    }
    return NULL;
}

void* consumer_binary(void* arg) {
    /*
        Función que simula un consumidor en el ejemplo de semáforo binario

        Entradas:
        - arg: Puntero a un entero que representa el identificador del consumidor

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    int id = *((int*) arg);
    while (consumed_items < NUM_ITEMS) {
        binary_semaphore_wait(&full);
        int item = buffer[out];
        printf("Consumer %d: consumed %d\n", id, item);
        out = (out + 1) % BUFFER_SIZE;
        consumed_items++;
        binary_semaphore_post(&empty);

        if (consumed_items >= NUM_ITEMS) {
            break;
        }
        sleep(1);
    }
    return NULL;
}

// Funciones para el ejemplo de semáforo de conteo: contador simple
void* increment_counter(void* arg) {
    /*
        Función que incrementa un contador compartido usando un semáforo de conteo

        Entradas:
        - arg: Puntero a un entero que representa el identificador del hilo

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    int id = *((int*) arg);
    for (int i = 0; i < INCREMENT_COUNT; i++) {
        counting_semaphore_wait(&counting_semaphore);
        if (shared_counter < 100) { 
            shared_counter++;
            printf("Thread %d incremented counter to %d\n", id, shared_counter);
        }
        counting_semaphore_post(&counting_semaphore);
        sleep(1);

        if (shared_counter == STOP_VALUE) {
            break;
        }
    }
    return NULL;
}

void* decrement_counter(void* arg) {
    /*
        Función que decrementa un contador compartido usando un semáforo de conteo

        Entradas:
        - arg: Puntero a un entero que representa el identificador del hilo

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    int id = *((int*) arg);
    for (int i = 0; i < DECREMENT_COUNT; i++) {
        counting_semaphore_wait(&counting_semaphore);
        if (shared_counter > 0) {
            shared_counter--;
            printf("Thread %d decremented counter to %d\n", id, shared_counter);
        }
        counting_semaphore_post(&counting_semaphore);
        sleep(1);
        if (shared_counter == STOP_VALUE) {
            break;
        }
    }
    return NULL;
}

// Funciones para el ejemplo de barrera
void* barrier_example(void* arg) {
    /*
        Función que espera en una barrera y luego continúa

        Entradas:
        - arg: Puntero a un entero que representa el identificador del hilo

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    int id = *((int*) arg);
    printf("Thread %d reached the barrier\n", id);
    barrier_wait(&barrier);
    printf("Thread %d passed the barrier\n", id);
    return NULL;
}

// Funciones para el ejemplo de Read/Write Lock
void* reader_thread_function(void* arg) {
    /*
        Función que lee de un buffer compartido usando un candado de lectura/escritura

        Entradas:
        - arg: Puntero a un entero que representa el identificador del lector

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido
    */
    long thread_id = (long)arg;
    char last_message[BUFFER_LENGTH] = "";

    while (1) {
        read_write_lock_read_lock(&read_write_lock);

        if (finished_writing && current_message_index >= NUM_STRINGS) {
            read_write_lock_unlock(&read_write_lock);
            break;
        }

        if (strcmp(shared_buffer, last_message) != 0) {
            printf("Reader %ld: %s\n", thread_id, shared_buffer);
            strcpy(last_message, shared_buffer);
        }

        read_write_lock_unlock(&read_write_lock);
        usleep(250000);
    }
    return NULL;
}

void slow_copy(char *destination, char *origin, int length) {
    /*
        Función que copia lentamente una cadena de caracteres de origin a destination

        Entradas:
        - destination: Puntero al buffer de destino
        - origin: Puntero a la cadena de origen
        - length: Longitud máxima a copiar

        Salidas:
        - Ninguna

        Restricciones:
        - Los punteros destination y origin deben ser válidos
        - La longitud debe ser mayor que 0
    */
    for (int i = 0; i < length; i++) {
        destination[i] = origin[i];
        if (destination[i] == '\0') break;
        usleep(50000);
    }
}

// Funciones para el ejemplo de mutex: Silla de espera
void* mutex_example(void* arg) {
    /*
        Función que simula a una persona que espera, usa y luego deja una silla de espera,
        utilizando un mutex para asegurar acceso exclusivo a la silla

        Entradas:
        - arg: Puntero a un entero que representa el identificador de la persona

        Salidas:
        - Ninguna

        Restricciones:
        - La variable arg debe apuntar a un entero válido que represente el identificador de la persona
        - El mutex debe estar correctamente inicializado antes de llamar a esta función
    */
    int person_id = *(int*)arg;

    printf("Person %d is waiting to use the chair...\n", person_id);
    sleep(rand() % 3); 

    mutex_lock(&mutex); 
    printf("Person %d is sitting on the chair.\n", person_id);
    sleep(2); 
    printf("Person %d is leaving the chair.\n", person_id);
    mutex_unlock(&mutex);

    free(arg);
    return NULL;
}

// Función principal main
int main(int argc, char *argv[]) {
    /*
        Función principal que gestiona la ejecución de diferentes ejemplos de sincronización

        Entradas:
        - argc: Número de argumentos en la línea de comandos
        - argv: Array de cadenas de caracteres que contiene los argumentos

        Salidas:
        - Código de salida del programa
    */
    if (argc != 2) {
        // Imprime el uso del programa si el número de argumentos es incorrecto
        printf("Usage: %s <barrier|binary_semaphore|counting_semaphore|mutex|read_write_lock>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "binary_semaphore") == 0) {
        // Configura y ejecuta el ejemplo de semáforo binario
        pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];
        int ids[NUM_PRODUCERS + NUM_CONSUMERS];

        binary_semaphore_init(&empty, BUFFER_SIZE);
        binary_semaphore_init(&full, 0);

        for (int i = 0; i < NUM_PRODUCERS; i++) {
            ids[i] = i;
            pthread_create(&producers[i], NULL, producer_binary, &ids[i]);
        }

        for (int i = 0; i < NUM_CONSUMERS; i++) {
            ids[i + NUM_PRODUCERS] = i;
            pthread_create(&consumers[i], NULL, consumer_binary, &ids[i + NUM_PRODUCERS]);
        }

        for (int i = 0; i < NUM_PRODUCERS; i++) {
            pthread_join(producers[i], NULL);
        }

        for (int i = 0; i < NUM_CONSUMERS; i++) {
            pthread_join(consumers[i], NULL);
        }

        binary_semaphore_destroy(&empty);
        binary_semaphore_destroy(&full);
    } else if (strcmp(argv[1], "counting_semaphore") == 0) {
        // Configura y ejecuta el ejemplo de semáforo de conteo
        pthread_t threads[NUM_THREADS];
        int ids[NUM_THREADS];

        counting_semaphore_init(&counting_semaphore, 1);

        for (int i = 0; i < NUM_THREADS / 2; i++) {
            ids[i] = i;
            pthread_create(&threads[i], NULL, increment_counter, &ids[i]);
        }

        for (int i = NUM_THREADS / 2; i < NUM_THREADS; i++) {
            ids[i] = i;
            pthread_create(&threads[i], NULL, decrement_counter, &ids[i]);
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        counting_semaphore_destroy(&counting_semaphore);

        printf("Final counter value: %d\n", shared_counter);
    } else if (strcmp(argv[1], "barrier") == 0) {
        // Configura y ejecuta el ejemplo de barrera
        pthread_t threads[THREADS];

        barrier_init(&barrier, THREADS);

        for (int i = 0; i < THREADS; i++) {
            ids[i] = i;
            pthread_create(&threads[i], NULL, barrier_example, &ids[i]);
        }

        for (int i = 0; i < THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        barrier_destroy(&barrier);
    } else if (strcmp(argv[1], "read_write_lock") == 0) {
        // Configura y ejecuta el ejemplo de Read/Write Lock
        pthread_t readers[NUM_READERS];

        read_write_lock_init(&read_write_lock);

        for (long i = 0; i < NUM_READERS; i++) {
            pthread_create(&readers[i], NULL, reader_thread_function, (void *)i);
        }

        while (current_message_index < NUM_STRINGS) {
            read_write_lock_write_lock(&read_write_lock);
            slow_copy(shared_buffer, messages[current_message_index], BUFFER_LENGTH);
            read_write_lock_unlock(&read_write_lock);
            current_message_index++;
            sleep(2);
        }

        read_write_lock_write_lock(&read_write_lock);
        finished_writing = 1;
        pthread_cond_broadcast(&read_write_lock.read);
        read_write_lock_unlock(&read_write_lock);

        for (int i = 0; i < NUM_READERS; i++) {
            pthread_join(readers[i], NULL);
        }

        read_write_lock_destroy(&read_write_lock);
    } else if (strcmp(argv[1], "mutex") == 0) {
        // Configura y ejecuta el ejemplo de mutex
        pthread_t threads[NUM_PERSONS];

        mutex_init(&mutex);

        for (int i = 0; i < NUM_PERSONS; i++) {
            int* person_id = malloc(sizeof(int));
            *person_id = i;
            pthread_create(&threads[i], NULL, mutex_example, person_id);
        }

        for (int i = 0; i < NUM_PERSONS; i++) {
            pthread_join(threads[i], NULL);
        }

        mutex_destroy(&mutex);
    } else {
        // Imprime el uso del programa si el argumento es inválido
        printf("Invalid argument. Usage: %s <barrier|binary_semaphore|counting_semaphore|mutex|read_write_lock>\n", argv[0]);
        return 1;
    }

    return 0;
}