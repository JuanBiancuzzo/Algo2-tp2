#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "heap.h"
#include "batallas.h"

#define MAX_NOMBRE 50
#define MAX_POKE_COMBATE 6
#define GIMNASIO "G"
#define LIDER "L"
#define POKEMON "P"
#define ENTRENADOR "E"
#define INGRESAR_ARCHIVO "E"
#define AGREGAR_GIMNASIO "A"
#define COMENZAR_PARTIDA "I"
#define SIMULAR_PARTIDA "S"
#define MOSTRAR_ENTRENADOR "E"
#define MOSTRAR_GIMNASIO "G"
#define CAMBIAR_POKEMON "C"
#define PROXIMA_BATALLA "B"
#define PROXIMO_COMBATE "N"

typedef int (* funcion_batalla)(void*, void *);

typedef int (* liberar_gimnasio)(void*, void *);

typedef int (* liberar_entrenador)(void*, void *);

typedef struct pokemon{
	int velocidad;
	int defensa;
	int ataque;
} pokemon_t;

typedef struct mapa {
	heap_t* gimnasios;
	int cant_gimnasios;
} mapa_t;

typedef struct gimnasio {
	char nombre[MAX_NOMBRE];
	int dificultad;
	int id_funcion;
	lista_t* entrenadores; // es una pila
	int cant_entrenadores;
} gimnasio_t;

typedef struct entrenador {
	char nombre[MAX_NOMBRE];
	lista_t* pokemones; // es una cola
	int cant_pokemones;
} entrenador_t;

typedef struct personaje {
	char nombre[MAX_NOMBRE];
	lista_t* pokemones; // es una lista
	int cant_pokemones;
} personaje_t;

int main() {


	return 0;
}
