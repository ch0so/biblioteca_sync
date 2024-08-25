# Nombre del ejecutable : libsync
# Version : 1.0
# Fecha de creación: Agosto 26, 2024
# Autores : 
#    - Ávila Ramírez Paublo Alexander (2022035584)
#    - Reyes Rodriguez Ricardo (2022101681)
#    - Zúñiga Campos Omar Jesús (2022019053)
# Descripción:
# Este Makefile está diseñado para compilar una biblioteca estática y un programa ejecutable
# en un entorno Fedora. La biblioteca estática incluye implementaciones de mutex, semáforo,
# barrera y lock de lectura/escritura. El ejecutable se enlaza con esta biblioteca para 
# demostrar su uso. 
# Importante tener en cuenta:
# - Asegúrate de que el compilador GCC y las herramientas de construcción básicas están instaladas
#   en tu sistema Fedora. Puedes instalar estas herramientas usando el siguiente comando:
#     sudo dnf install gcc make
#
# - El Makefile asume que los archivos fuente (.c) y de cabecera (.h) están en el mismo directorio
#   que el Makefile. Si usas subdirectorios, ajusta las rutas en las variables SRCS y HDRS.
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
SRCS = mutex.c barrier.c semaphore.c rwlock.c main.c
OBJS = $(SRCS:.c=.o)

# Archivos de cabecera
HDRS = mutex.h barrier.h semaphore.h rwlock.h

# Nombre de la biblioteca y el ejecutable
LIBRARY = libsync.a
EXECUTABLE = homework_sync_library

# Objetivo por defecto
all: $(EXECUTABLE)

# Construir el ejecutable
$(EXECUTABLE): $(OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L. -lsync -pthread

# Construir la biblioteca estática
$(LIBRARY): mutex.o semaphore.o barrier.o rwlock.o
	ar rcs $@ $^

# Compilar archivos .c en archivos .o
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(EXECUTABLE) $(LIBRARY)

.PHONY: all clean


