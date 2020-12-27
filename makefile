CC=gcc
VAL=valgrind
NAME=journey
CFLAGS=-g -Wall -Wconversion -Wtype-limits -pedantic -Werror -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes

build:
	$(CC) *.c -o $(NAME) $(CFLAGS)

exe: build
	./$(NAME)

valgrind: build
	clear
	$(VAL) $(VFLAGS) ./pruebas/$(NAME)
