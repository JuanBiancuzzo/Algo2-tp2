CC=gcc
VAL=valgrind
NAME=aventura
LIB=librerias/*.c
CFLAGS=-g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

build:
	$(CC) $(CFLAGS) $(LIB) main.c -o $(NAME)

exe: build
	./$(NAME)

default: build
	./$(NAME) default

build_v:
	$(CC) $(CFLAGS) $(LIB) pruebas/pruebas.c -o $(NAME)

valgrind: build_v
	$(VAL) $(VFLAGS) ./$(NAME)
