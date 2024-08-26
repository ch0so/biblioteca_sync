# Biblioteca Sync

## Grupo

- Ávila Ramírez Paublo Alexander (2022065584)
- Reyes Rodríguez Ricardo Andrés (2022101681)
- Zúñiga Campos Omar Jesús (2022019053)


## Introducción

Biblioteca Sync usando hilos y wait/notify que fue implementada en C e incorpora las siguientes estructuras de sincronización:

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

A continuación, se presentan las funciones presentes en la implementación de barrera:

### 1. Inicialización:
   ```c
    void barrier_init(barrier_t *barrier, int threshold);
   ```
   Función que inicializa la barrera con el número de hilos que deben sincronizarse.

### 2. Esperar en la Barrera:
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

A continuación, se presentan las funciones presentes en la implementación de barrera:

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
No devuelve un valor como tal, solo un NULL que esto se debe más que nada a la sintáxis vista en clase.

#### Restricciones:
La variable arg debe apuntar a un entero válido.

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
Se emplea barrier_init() para inicializar la barrera con el número de hilos de la variable global.

#### Creación:
Se crean THREADS hilos, cada uno ejecutando la función barrier_example. En el código se puede ver que THREADS en este caso son 10 hilos.

#### Espera de hilos:
pthread_join se encarga de esperar a que todos los hilos terminen su ejecución.

#### Destrucción:
barrier_destroy al final libera los recursos asociados con la barrera.

#### 3. Ejecución del ejemplo:
Para poder ejecutar el ejemplo de barrera, ejecute estos pasos:

##### 1. Compile el código:
Diríjase al directorio en el que se descargó el proyecto y ejecute el siguiente comando:
   ```bash
   make
   ```
##### 2. Ejecute el ejemplo:
Use el siguiente comando para ejecutar el programa::
   ```bash
   ./homework_sync_library barrier
   ```
Donde homework_sync_library es el nombre del ejecutable que se genero en el directorio donde ejecutó el comando make, y barrier es el argumento para indicarle al programa principal que debe correr el ejemplo de barrera.