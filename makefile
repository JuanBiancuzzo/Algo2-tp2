CC=gcc
VAL=valgrind
NAME=journey
LIB=librerias/heap.c librerias/lista.c librerias/batallas.c librerias/herramientas.c
CFLAGS=-g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

build:
	$(CC) $(LIB) main.c -o $(NAME) $(CFLAGS)

exe: build
	./$(NAME)

valgrind:
	$(CC) $(LIB) pruebas/pruebas.c -o $(NAME) $(CFLAGS)
	$(VAL) $(VFLAGS) ./$(NAME)
