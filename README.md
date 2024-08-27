# Biblioteca Sync

## Grupo

- Ávila Ramírez Paublo Alexander (2022065584)
- Reyes Rodríguez Ricardo Andrés (2022101681)
- Zúñiga Campos Omar Jesús (2022019053)

## Introducción

Biblioteca Sync usando hilos y mutex, la cual fue implementada en C usando pthread e incorpora las siguientes estructuras de sincronización:

- Mutex
- Semáforo
- Barrera
- Read/write lock

Además, incluye ejemplos de prácticos de uso para cada uno para un mejor entendimiento.

## Instalación

1. **Instale Fedora Workstation 40**
   En el caso de no contar con Fedora Workstation 40, debe dirigirse a la página oficial e instalar el sistema operativo que se encuentra en el siguiente enlace:

```bash
     https://fedoraproject.org/es/workstation/download/
```

En el link suminstrado en el anterior punto, puede encontrar la documentación oficial que le ayudará a realizar el proceso de instalación adecuadamente.

2. **Instale GCC y make**
   Es necesario que se asegure de que el compilador GCC y las herramientas de construcción básicas estén instaladas en un sistema Fedora Workstation 40. Puede instalar las herramientas necesarias utilizando el siguiente comando:

```bash
sudo dnf install gcc make
```

3. **Clone el repositorio**:

   ```bash
   git clone https://github.com/ch0so/biblioteca_sync.git
   ```

4. **Compile la biblioteca**:
   Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

   ```bash
   make
   ```

5. **Ejecute los ejemplos**:
   Para ejecutar cada uno de los ejemplos, lo que debe hacer es escribir el siguiente comando:
   ```bash
   ./homework_sync_library <barrier|binary_semaphore|counting_semaphore|mutex|read_write_lock>
   ```
   Tenga en cuenta que solo debe escribir la opción que desea usar, por lo que un ejemplo concreto para correr el ejemplo de barrera sería el siguiente:
   ```bash
   ./homework_sync_library barrier
   ```

# Implementación de barrera

## Qué es una barrera

Es una herramienta empleado en sincronización que fuerza a un grupo de hilos a que esperen hasta que todos hayan podido llegar a un punto en concrento antes de que puedan proseguir (Desai, 2020).

## Estructura para la Barrera

La estructura barrier_t define una barrera utilizando un mutex y una variable de condición. Aquí está su definición:

```c
     typedef struct {
         pthread_mutex_t mutex;
         pthread_cond_t condition;
         int count;
         int threshold;
     } barrier_t;
```

Los componentes serían los siguientes:

### 1. mutex

Un mutex que se emplea para asegurar el acceso exclusivo al contador de la barrera. Esto evita condiciones de carrera al actualizar el estado compartido.

### 2. condition

Una variable de condición se utiliza para el manejo de la sincronización de los hilos en la barrera. Permite que los hilos esperen hasta que se cumpla una condición específica, en este caso, el número necesario de hilos para cruzar la barrera.

### 3. count

El contador lleva el registro del número actual de hilos que han llegado a la barrera. Se incrementa cada vez que un hilo llega a la barrera y se utiliza para determinar cuándo se puede liberar a todos los hilos.

### 4. threshold

El umbral define el número de hilos necesarios para cruzar la barrera. Una vez que el número de hilos que han llegado a la barrera alcanza este umbral, todos los hilos se desbloquean y pueden continuar su ejecución.

## Funciones de Barrera

A continuación, se presentan las funciones usadas en la implementación de barrera:

### 1. Inicialización:

```c
 void barrier_init(barrier_t *barrier, int threshold);
```

Función que inicializa la barrera con el número de hilos que deben sincronizarse.

### 2. Espera:

```c
 void barrier_wait(barrier_t *barrier);
```

Función que espera a que todos los hilos alcancen la barrera.

### 3. Destrucción:

```c
 void barrier_destroy(barrier_t *barrier);
```

Función que destruye la barrera, liberando los recursos utilizados.

## Ejemplo utilizado para la barrera

A continuación, se presentan las funciones que forman parte de la implementación del ejemplo de barrera que consiste en que los hilos llegan a una barrera y se quedan ahí hasta que puedan pasar:

### 1. Función barrier_example

```c
void* barrier_example(void* arg) {
   int id = *((int*) arg);
   printf("Thread %d reached the barrier\n", id);
   barrier_wait(&barrier);
   printf("Thread %d passed the barrier\n", id);
   return NULL;
}
```

#### Descripción de la función:

Función en la que cada hilo imprime un mensaje para indicar que ha alcanzado a la barrera, luego espera a que todos los hilos lleguen a la barrera por medio de barrier_wait, y luego imprime un mensaje para informar que ha pasado la barrera.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del hilo.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 2. Creación y sincronización de hilos

```c
   #define THREADS 10

   barrier_t barrier;

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
```

#### Inicialización:

Se emplea `barrier_init` para inicializar la barrera con el número de hilos de la variable global.

#### Creación:

Se crean `THREADS` hilos, cada uno ejecutando la función `barrier_example`. En el código se puede ver que `THREADS` en este caso son 10 hilos.

#### Espera de hilos:

`pthread_join` se encarga de esperar a que todos los hilos terminen su ejecución.

#### Destrucción:

`barrier_destroy` al final libera los recursos asociados con la barrera.

### 3. Ejecución del ejemplo:

Para poder ejecutar el ejemplo de barrera, ejecute estos pasos:

##### 1. Compile el código:

Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

```bash
make
```

##### 2. Ejecute el ejemplo:

Use el siguiente comando para ejecutar el programa:

```bash
./homework_sync_library barrier
```

Donde `homework_sync_library` es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando `make`, y `barrier` es el argumento para indicarle al programa principal que debe correr el ejemplo de barrera.

# Implementación de semáforo binario

## Qué es un semáforo binario

Es un tipo de semáforo el cual cuenta con solo dos únicos estados: Bloqueado y desbloqueado, y es empleado para el control de acceso a un recurso compartido por una sola tarea a la vez (GeeksforGeeks, 2024).

## Estructura para el semáforo binario

La estructura `binary_semaphore_t` define un semáforo binario utilizando un mutex y una variable de condición. Aquí está su definición:

```c
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t conditional;
    int value;
} binary_semaphore_t;
```

Los componentes serían los siguientes:

### 1. mutex

Mutex para proteger el acceso al semáforo.

### 2. conditional

Variable de condición para sincronización.

### 3. value

Número entero que indica el estado del semáforo, donde 0 es para bloqueado y 1 para desbloqueado.

## Funciones de Semáforo Binario

A continuación, se presentan las funciones usadas en la implementación de semáforo binario:

### 1. Inicialización:

```c
void binary_semaphore_init(binary_semaphore_t *binary_semaphore, int value);
```

Función que inicializa un semáforo binario con un valor inicial.

### 2. Espera:

```c
void binary_semaphore_wait(binary_semaphore_t *binary_semaphore);
```

Función que realiza la operación de espera en el semáforo binario, bloqueando si el valor es 0 hasta que se convierte en 1.

### 3. Señalización:

```c
void binary_semaphore_post(binary_semaphore_t *binary_semaphore);
```

Función que realiza la operación de señalización en el semáforo binario, cambiando su valor a 1 y despertando a un hilo en espera.

### 4. Destrucción:

```c
void binary_semaphore_destroy(binary_semaphore_t *binary_semaphore);
```

Función que destruye el semáforo binario, liberando los recursos utilizados para el mutex y la variable de condición.

## Ejemplo utilizado para el semáforo binario

A continuación, se presentan las funciones usadas en la implementación del ejemplo de semáforo binario que consiste en que hay productores que generan un producto y luego están los consumidores que consumen los productos generados:

### 1. Función producer_binary

```c
void* producer_binary(void* arg) {
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
```

#### Descripción de la función:

Función que simula un productor en el ejemplo de semáforo binario.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del productor.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 2. Función consumer_binary

```c
void* consumer_binary(void* arg) {
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
```

#### Descripción de la función:

Función que simula a un consumidor en el ejemplo de semáforo binario.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del consumidor.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 3. Creación y sincronización de hilos

```c
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 4
#define NUM_CONSUMERS 4
#define NUM_ITEMS 20

int produced_items = 0;
int consumed_items = 0;

binary_semaphore_t empty;
binary_semaphore_t full;

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
```

#### Inicialización:

Se emplea `binary_semaphore_init` para inicializar el semáforo binario que se utiliza, que en este caso serían dos: Los que están llenos y los que están vacíos.

#### Creación:

Se crean `NUM_PRODUCERS` productores, cada uno ejecutando la función `producer_binary`. En el código se puede ver que `NUM_PRODUCERS` en este caso son 4 productores.

A su vez, se crean `NUM_CONSUMERS` consumidores, cada uno ejecutando la función `producer_binary`. En el código se puede ver que `NUM_CONSUMERS` en este caso son 4 consumidores.

#### Espera de hilos:

`pthread_join` se encarga de esperar a que todos los hilos terminen su ejecución.

#### Destrucción:

`binary_semaphore_destroy` al final libera los recursos asociados con el semáforo binario.

### 4. Ejecución del ejemplo:

Para poder ejecutar el ejemplo de semáforo binario, ejecute estos pasos:

##### 1. Compile el código:

Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

```bash
make
```

##### 2. Ejecute el ejemplo:

Use el siguiente comando para ejecutar el programa:

```bash
./homework_sync_library binary_semaphore
```

Donde `homework_sync_library` es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando `make`, y `binary_semaphore` es el argumento para indicarle al programa principal que debe correr el ejemplo de semáforo binario.

# Implementación de semáforo de conteo

## Qué es un semáforo de conteo

Este tipo de semáforo es el que permite la gestión de una cantidad específica de recursos compartidos, siendo así que da la posibilidad a varios hilos de adquirir y liberar el recurso hasta un límite ya establecido (GeeksforGeeks, 2024).

## Estructura para el semáforo de conteo

La estructura `counting_semaphore_t` define un semáforo binario utilizando un mutex y una variable de condición. Aquí está su definición:

```c
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t conditional;
    int value;
} counting_semaphore_t;
```

Los componentes serían los siguientes:

### 1. mutex

Mutex para proteger el acceso al semáforo.

### 2. conditional

Variable de condición para sincronización entre hilos.

### 3. value

Número entero que indica el contador del semáforo.

## Funciones de semáforo de conteo

A continuación, se presentan las funciones usadas en la implementación de semáforo de conteo:

### 1. Inicialización:

```c
void counting_semaphore_init(counting_semaphore_t *counting_semaphore, int value);
```

Función que inicializa un semáforo de conteo con un valor inicial.

### 2. Espera:

```c
void counting_semaphore_wait(counting_semaphore_t *counting_semaphore);
```

Función que decrementa el contador del semáforo de conteo, bloqueando si no hay recursos disponibles.

### 3. Señalización:

```c
void counting_semaphore_post(counting_semaphore_t *counting_semaphore);
```

Función que incrementa el contador del semáforo de conteo, señalando a cualquier hilo bloqueado que puede continuar

### 4. Destrucción:

```c
void counting_semaphore_destroy(counting_semaphore_t *counting_semaphore);
```

Función que destruye el semáforo de conteo, liberando los recursos asociados.

## Ejemplo utilizado para el semáforo de conteo

A continuación, se presentan las funciones usadas en la implementación del ejemplo de semáforo de conteo que hay un contador compartido que se aumenta hasta 25 y luego baja hasta 20 para así finalizar el programa:

### 1. Función increment_counter

```c
void* increment_counter(void* arg) {
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
```

#### Descripción de la función:

Función que incrementa un contador compartido usando un semáforo de conteo.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del hilo.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 2. Función decrement_counter

```c
void* decrement_counter(void* arg) {
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
```

#### Descripción de la función:

Función que decrementa un contador compartido usando un semáforo de conteo.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del hilo.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 3. Creación y sincronización de hilos

```c
#define INITIAL_COUNTER 20
#define INCREMENT_COUNT 5
#define DECREMENT_COUNT 5
#define STOP_VALUE 20
#define NUM_PERSONS 5
#define NUM_THREADS 10

counting_semaphore_t counting_semaphore;

int shared_counter = INITIAL_COUNTER;

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
```

#### Inicialización:

Se emplea `counting_semaphore_init` para inicializar el semáforo de conteo.

#### Creación:

Se crean `NUM_THREADS` hilos, que este caso serían 10 hilos que se están ejecutando para hacer el proceso de conteo más rápido. La variable global indica que son 10 hilos que están siendo utilizados en total. Además, se tienen los ids para identificar el número de hilo y así tener un mejor orden.

#### Espera de hilos:

`pthread_join` se encarga de esperar a que todos los hilos terminen su ejecución.

#### Destrucción:

`counting_semaphore_destroy` al final libera los recursos asociados con el semáforo de conteo.

### 4. Ejecución del ejemplo:

Para poder ejecutar el ejemplo de semáforo de conteo, ejecute estos pasos:

##### 1. Compile el código:

Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

```bash
make
```

##### 2. Ejecute el ejemplo:

Use el siguiente comando para ejecutar el programa:

```bash
./homework_sync_library counting_semaphore
```

Donde `homework_sync_library` es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando `make`, y `counting_semaphore` es el argumento para indicarle al programa principal que debe correr el ejemplo de semáforo de conteo.

# Implementación de Read/Write Lock

## Qué es un Read/Write Lock

Un read/write lock es un sistema de sincronización que permite a múltiples hilos leer datos compartidos de manera concurrente, pero restringe la escritura a un único hilo a la vez (IBM, 2023).

## Estructura para el Read/Write Lock

La estructura `read_write_lock_t` define un read/write lock utilizando un mutex, dos variables de condición, dos contadores, y una bandera. Aquí está su definición:

```c
   typedef struct {
      pthread_mutex_t mutex;    
      pthread_cond_t read;        
      pthread_cond_t write;        
      int readers;                 
      int writers;              
      int writing_flag;            
   } read_write_lock_t;
```

Los componentes serían los siguientes:

### 1. mutex

Un mutex que se emplea para asegurar el acceso exclusivo al candado de lectura/escritura. Esto evita condiciones de carrera al actualizar el estado compartido.

### 2. read

Una variable de condición se utiliza para el manejo de la sincronización de los lectores en el candado de lectura/escritura. Permite que los hilos lectores esperen hasta que se cumpla una condición específica.

### 3. write

Una variable de condición se utiliza para el manejo de la sincronización de los escritores en el candado de lectura/escritura. Permite que los hilos escritores esperen hasta que se cumpla una condición específica.

### 4. readers

El contador lleva el registro del número actual de lectores que están leyendo. Se incrementa cada vez que un lector comienza a leer y se decrementa cuando termina.

### 5. writers

El contador lleva el registro del número actual de escritores que están esperando para escribir. Se incrementa cada vez que un escritor intenta escribir y se decrementa cuando termina.

### 6. writing_flag

El indicador señala si un escritor está escribiendo actualmente. Si es 1, significa que un escritor está escribiendo, de lo contrario, es 0.

## Funciones de Read/Write Lock

A continuación, se presentan las funciones usadas en la implementación de read/write lock:

### 1. Inicialización:

```c
   void read_write_lock_init(read_write_lock_t *read_write_lock);
```

Función que inicializa un read/write lock.

### 2. Leer:

```c
   void read_write_lock_read_lock(read_write_lock_t *read_write_lock);
```

Función que adquiere un candado de lectura.

### 3. Escribir:

```c
   void read_write_lock_write_lock(read_write_lock_t *read_write_lock);
```

Función que adquiere un candado de escritura.

### 4. Liberar:

```c
   void read_write_lock_unlock(read_write_lock_t *read_write_lock);
```

Función que libera un candado de lectura/escritura.

### 5. Destrucción:

```c
   void read_write_lock_destroy(read_write_lock_t *read_write_lock);
```

Función que destruye el read/write lock, liberando los recursos utilizados.

## Ejemplo utilizado para el Read/Write Lock

A continuación, se presentan las funciones usadas en la implementación del ejemplo de read/write lock que consiste en que hay lectores y escritores que acceden a un recurso compartido:

### 1. Función reader_thread_function

```c
void* reader_thread_function(void* arg) {
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
```

#### Descripción de la función:

Esta función define el comportamiento de un hilo lector que cicla infinitamente. Se inicia al hacer un read lock para la lectura del buffer, esta es una variable en donde los escritores van a escribir mensajes obtenidos del array messages. Después de leer, se imprime lo leído y se hace un usleep(250000) para simular una lectura extensiva. Finalmente, se hace un usleep(500000); para limitar el uso de los lectores.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador del hilo.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:

La variable `arg` debe apuntar a un entero válido.

### 2. Función slow_copy

```c
void slow_copy(char *destination, char *origin, int length) {
    for (int i = 0; i < length; i++) {
        destination[i] = origin[i];
        if (destination[i] == '\0') break;
        usleep(50000);
    }
}
```

#### Descripción de la función:

Función que copia lentamente una cadena de caracteres de origin a destination.

#### Entradas:

##### destination:

Puntero al buffer de destino.

##### origin:

Puntero a la cadena de origen.

##### length:

Longitud máxima a copiar.

#### Salidas:

No retorna nada.

#### Restricciones:
##### Primera restricción:
      Los punteros destination y origin deben ser válidos.
##### Segunda restricción:
      La longitud debe ser mayor que 0.

### 3. Creación de hilos de lectura y escritura

```c
   #define NUM_READERS 5

   read_write_lock_t read_write_lock;

   char shared_buffer[BUFFER_LENGTH] = {0};
   char *messages[NUM_STRINGS] = {
      "Cadena de texto #1",
      "Segunda cadena de texto",
      "Ahora es la tercera cadena de texto",
      "Cadena de texto final del ejemplo",
   };

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
```

### Inicialización:

Se emplea `read_write_lock_init` para inicializar el read/write lock.

### Creación de hilos de lectura:

Se crean `NUM_READERS` hilos lectores, cada uno ejecutando la función `reader_thread_function`. En el código se puede ver que `NUM_READERS` en este caso son 5 lectores.

### Creación de hilos de escritura:

Se crea un hilo escritor que escribe en el buffer utilizando `slow_copy`. Este utiliza el `read_write_lock_write_lock` para adquirir un candado de escritura y `read_write_lock_unlock` para liberarlo.

### Espera de hilos:

`pthread_join` se encarga de esperar a que todos los hilos terminen su ejecución.

### Destrucción:

`read_write_lock_destroy` al final libera los recursos asociados con el read/write lock.

### 4. Ejecución del ejemplo:

Para poder ejecutar el ejemplo de read/write lock, ejecute estos pasos:

#### 1. Compile el código:

Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

```bash
make
```

#### 2. Ejecute el ejemplo:

Use el siguiente comando para ejecutar el programa:

```bash
./homework_sync_library read_write_lock
```

Donde `homework_sync_library` es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando `make`, y `read_write_lock` es el argumento para indicarle al programa principal que debe correr el ejemplo de read/write lock.

# Implementación de mutex

## Qué es un mutex

Un mutex, que es una abreviatura de mutual exclusion o exclusión mutua en español, es un mecanismo empleado en la programación concurrente para asegurar que múltiples procesos o hilos no accedan de manera simultánea a un recurso compartido, siendo este el caso de una variable o región crítica de código, lo cual se consigue al bloquear el acceso al recurso al ser utilizado por un proceso, siendo así que otros procesos no pueden acceder a ese recurso hasta que sea liberado (IBM, 2023).

## Estructura para el mutex

La estructura mutex_t define un mutex utilizando el mutex de la biblioteca de pthread:

```c
   typedef struct {
      pthread_mutex_t mutex; 
   } mutex_t;
```

Los componentes serían los siguientes:

### 1. mutex

Un mutex que se utiliza para la sincronización de hilos.

## Funciones de mutex

A continuación, se presentan las funciones usadas en la implementación de mutex:

### 1. Inicialización:

```c
 void mutex_init(mutex_t *parameter_mutex);
```

Función que inicializa un mutex.

### 2. Bloqueo:

```c
 void mutex_lock(mutex_t *parameter_mutex);
```
Función que bloquea un mutex para evitar condiciones de carrera.

### 3. Desbloqueo:

```c
void mutex_unlock(mutex_t *parameter_mutex);
```
Función que desbloquea un mutex, permitiendo a otros hilos acceder a la sección crítica.

### 4. Destrucción:

```c
void mutex_destroy(mutex_t *parameter_mutex);
```

Función que destruye el mutex, liberando los recursos utilizados.

## Ejemplo utilizado para el mutex

A continuación, se presentan las funciones que forman parte de la implementación del ejemplo de mutex que consiste en que se simula a una persona que espera, usa y luego deja una silla de espera, utilizando un mutex para asegurar acceso exclusivo a la silla:

### 1. Función barrier_example

```c
void* mutex_example(void* arg) {
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
```

#### Descripción de la función:

Función que simula a una persona que espera, usa y luego deja una silla de espera, utilizando un mutex para asegurar acceso exclusivo a la silla.

#### Entradas:

##### arg:

Puntero a un entero que representa el identificador de la persona.

#### Salidas:

No retorna un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:
##### Primera restricción:
      La variable arg debe apuntar a un entero válido que represente el identificador de la persona.
##### Segunda restricción:
      El mutex debe estar correctamente inicializado antes de llamar a esta función.

### 2. Creación y sincronización de hilos

```c
   #define NUM_PERSONS 5

   mutex_t mutex; 

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
```

#### Inicialización:

Se emplea `mutex_init` para la inicialización del mutex.

#### Creación:

Se crean `NUM_PERSONS` hilos, cada uno ejecutando la función `mutex_example`. En el código se puede ver que `NUM_PERSONS` en este caso son 5 hilos.

#### Espera de hilos:

`pthread_join` se encarga de esperar a que todos los hilos terminen su ejecución.

#### Destrucción:

`barrier_destroy` al final libera los recursos asociados con el mutex.

### 3. Ejecución del ejemplo:

Para poder ejecutar el ejemplo de mutex, ejecute estos pasos:

##### 1. Compile el código:

Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:

```bash
make
```

##### 2. Ejecute el ejemplo:

Use el siguiente comando para ejecutar el programa:

```bash
./homework_sync_library mutex
```

Donde `homework_sync_library` es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando `make`, y `mutex` es el argumento para indicarle al programa principal que debe correr el ejemplo de mutex.

## Referencias bibliográficas
Toda esta implementación así como este README fue elaborado gracias a estas referencias bibliográficas:
- Ansari, F. (2023, 18 mayo). Best practices for an eye catching GitHub Readme [Las mejores prácticas para un Readme de GitHub actractivo a la vista]. Hatica. https://www.hatica.io/blog/best-practices-for-github-readme/
- Desai, J. (2020, 11 junio). Barrier Synchronization in Threads [Sincronización de Barreras en Hilos]. Medium. https://medium.com/@jaydesai36/barrier-synchronization-in-threads-3c56f947047
- GeeksforGeeks. (2023, 9 mayo). Thread functions in C/C++ [Funciones de Hilos en C/C++]. GeeksforGeeks. https://www.geeksforgeeks.org/thread-functions-in-c-c/
- GeeksforGeeks. (2024, 1 agosto). Semaphores in Process Synchronization [Semáforos en Sincronización de Procesos]. GeeksforGeeks. https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
- IBM. (2023, marzo 24). Using mutexes [Uso de Mutexes]. IBM. https://www.ibm.com/docs/pt-br/aix/7.2?topic=programming-using-mutexes
- IBM. (2023, marzo 24). Using read/write locks [Uso de Bloqueos de Lectura/Escritura]. https://www.ibm.com/docs/en/aix/7.2?topic=programming-using-readwrite-locks