CC=gcc
VAL=valgrind
NAME=journey
LIB=librerias/*.c
CFLAGS=-g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

build:
	$(CC) $(LIB) main.c -o $(NAME) $(CFLAGS)

exe: build
	clear
	./$(NAME)

default: build
	clear
	./$(NAME) default

archivos: build
	clear
	./$(NAME) ash.txt ciudad_celeste.txt

valgrind:
	clear
	$(CC) $(LIB) pruebas/pruebas.c -o $(NAME) $(CFLAGS)
	$(VAL) $(VFLAGS) ./$(NAME)
