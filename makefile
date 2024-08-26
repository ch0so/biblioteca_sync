# Nombre del ejecutable : homework_sync_library
#
# Versión : 1.0
#
# Fecha de creación: Agosto 26, 2024
#
# Autores : 
#    - Ávila Ramírez Paublo Alexander (2022035584)
#    - Reyes Rodriguez Ricardo Andrés (2022101681)
#    - Zúñiga Campos Omar Jesús       (2022019053)
#
# Descripción:
# Este Makefile está diseñado para compilar una biblioteca estática y un programa ejecutable
# en un entorno Fedora. La biblioteca estática incluye implementaciones de mutex, semáforo binario,
# semáforo de conteo, barrera y candado de lectura/escritura. 
# El ejecutable se enlaza con esta biblioteca para demostrar su uso. 
#
# Importante tener en cuenta:
# - Es necesario que se asegure que el compilador GCC y las herramientas de construcción básicas están instaladas
#   en un sistema de Fedora Workstation 40. Se pueden instalar las herramientas necesarias usando el siguiente comando:
#     sudo dnf install gcc make
# 
# - En el caso de no contar con Fedora WOrkstation 40, debe dirigirse a la página oficial e instalar el sistema operativo
#   que se encuentra en el siguiente enlace:
#     https://fedoraproject.org/es/workstation/download/
# 
# - En el link suminstrado en el anterior punto, puede encontrar la documentación oficial que le ayudará a realizar el proceso
#   de instalación adecuadamente.
#
# - El Makefile asume que los archivos fuente (.c) y de cabecera (.h) están en el mismo directorio
#   que el Makefile. Si se usan subdirectorios, se deben ajustar las rutas en las variables SRCS y HDRS.
#
# - La biblioteca estática se crea con el nombre 'libsync.a' y se enlaza con el ejecutable llamado
#   'homework_sync_library'.
#
# Referencias bibliográficas:
# 	-GNU. (s.f.). GNU Make Manual. Recuperado de  https://www.gnu.org/software/make/manual/make.html
#*************************************************

# Compilador a usar
CC = gcc

# Banderas del compilador
CFLAGS = -Wall -g

# Archivos de código fuente y objeto
SRCS = barrier.c binary_semaphore.c counting_semaphore.c mutex.c read_write_lock.c main.c
OBJS = $(SRCS:.c=.o)

# Archivos de cabecera
HDRS = barrier.h binary_semaphore.h counting_semaphore.h mutex.h read_write_lock.h

# Nombre de la biblioteca y el ejecutable
LIBRARY = libsync.a
EXECUTABLE = homework_sync_library

# Objetivo por defecto
all: $(EXECUTABLE)

# Construir el ejecutable
$(EXECUTABLE): $(OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lsync -pthread

# Construir la biblioteca estática
$(LIBRARY): $(OBJS)
	ar rcs $@ $^

# Compilar archivos .c en archivos .o
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(LIBRARY)

.PHONY: all clean