#ifndef __HERRAMIENTAS_H__
#define __HERRAMIENTAS_H__

#include "lista.h"
#include "heap.h"

#define MAX_NOMBRE 100
#define MAX_POKE_COMBATE 6
#define MAX_LEVEL_UP 63
#define IGNORAR 1
#define EXITO 0
#define ERROR -1

#define GIMNASIO 'G'
#define LIDER 'L'
#define POKEMON 'P'
#define ENTRENADOR 'E'

typedef int (* funcion_batalla)(void*, void *);

typedef struct pokemon{
	char nombre[MAX_NOMBRE];
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
	lista_t* pokemones; // es una lista
	int cant_pokemones;
} entrenador_t;

typedef struct personaje {
	char nombre[MAX_NOMBRE];
	lista_t* pokemones; // es una lista
	int cant_pokemones;
} personaje_t;

/*
 * Apartir del ruta del archivo y un personaje vacio, carga al personaje
 * con los datos del archivo y devuelve EXITO si los cargo correctamente,
 * y ERROR si no lo logra
 */
int archivo_2_personaje_principal (char ruta_archivo[], void* principal);

/*
 * Apartir del ruta del archivo y un gimnasio vacio, carga el gimnasio
 * con los datos del archivo y devuelve EXITO si los cargo correctamente,
 * y ERROR si no lo logra
 */
int archivo_2_gimnasio (char ruta_archivo[], void* gimnasio);

/*
 * Se encarga de reservar memoria en el heap para el personaje principal
 * y devolver esa direccion, en caso de que no lo logre devuelve NULL
 */
personaje_t* crear_personaje_principal();

/*
 * Se encarga de reservar memoria en el heap para el gimnasio y devuelve
 * esa direccion, en caso de que no lo logre devuelve NULL
 */
gimnasio_t* crear_gimnasio();

/*
 * Se encarga de reservar memoria en el heap para el mapa y devuelve esa
 * direccion, en caso de que no lo logre devuelve NULL
 */
mapa_t* crear_mapa();

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el personaje, esto incluye todos sus pokemones
 */
void destruir_personaje_principal(personaje_t* principal);

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el gimnasio, esto incluye todos sus entrenadores que a su vez
 * tiene todos sus pokemones
 */
void destruir_gimnasio(gimnasio_t* gimnasio);

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el mapa
 */
void destruir_mapa(mapa_t* mapa);

/*
 * Agrega el gimnasio al mapa, que va a posicionarse de menor
 * dificultad a mayor
 */
int gimnasio_2_mapa(mapa_t* mapa, gimnasio_t* gimnasio);

/*
 * Dados el personaje principal y entrenador o lider, se hace la
 * batalla, devolviendo 1 si el personaje principal gana o -1 si
 * gano el entrenador o lider
 */
int batalla_pokemon(personaje_t* principal, entrenador_t* enemigo, funcion_batalla estilo);

/*
 * Dado un pokemon prestado del enemigo, se agrega a la lista de
 * pokemones del personaje pricipal, si tiene menos de
 * MAX_POKE_COMBATE (6) se agrega al final, si tiene mas se
 * agrega como septimo pokemon
 *
 * El id_pokemon va desde 0 siendo el primero pokemon hasta la
 * cantidad de pokemones del enemigo menos uno
 */
int tomar_prestado(personaje_t* principal, entrenador_t* enemigo, int id_pokemon);

/*
 * Dado una lista de pokemones, intercambia los pokemones dado por
 * su id
 */
int reordenar_pokemones(lista_t* pokemones, int pkm_uno, int pkm_dos);

#endif /* __HERRAMIENTAS_H__ */
