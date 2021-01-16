CC=gcc
VAL=valgrind

NAME=aventura
VNAME=prueba

CFLAGS=-g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

LIB=librerias/*.c

build:
	$(CC) $(CFLAGS) $(LIB) main.c -o $(NAME)

exe: build
	./$(NAME)

default: build
	./$(NAME) default

vbuild:
	$(CC) $(CFLAGS) $(LIB) pruebas/pruebas.c -o $(VNAME)

valgrind: vbuild
	$(VAL) $(VFLAGS) ./$(VNAME)
