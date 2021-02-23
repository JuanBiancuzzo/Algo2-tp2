#ifndef __HERRAMIENTAS_H__
#define __HERRAMIENTAS_H__

#include "lista.h"
#include "heap.h"

#define MAX_NOMBRE 100
#define MAX_FRASE 200
#define MAX_POKEMON_COMBATE 6
#define MAX_LEVEL_UP 63
#define IGNORAR 1
#define EXITO 0
#define ERROR -1

#define GIMNASIO 'G'
#define LIDER 'L'
#define POKEMON 'P'
#define ENTRENADOR 'E'

typedef int (*funcion_batalla)(void*, void *);

typedef struct mapa {
	heap_t* gimnasios;
} mapa_t;

typedef struct gimnasio {
	char nombre[MAX_NOMBRE];
	int dificultad;
	int id_funcion;
	lista_t* entrenadores; // una pila de entrenadores
} gimnasio_t;

typedef struct entrenador {
	char nombre[MAX_NOMBRE];
	lista_t* pokemones; // una lista de pokemones
	bool lider;
} entrenador_t;

typedef struct pokemon{
	char nombre[MAX_NOMBRE];
	int velocidad;
	int defensa;
	int ataque;
} pokemon_t;

/*
 * Se encarga de reservar memoria en el heap para el personaje principal
 * y devolver esa direccion, en caso de que no lo logre devuelve NULL
 */
entrenador_t* crear_entrenador();

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
 * Apartir de la ruta del archivo, carga el personaje principal con los
 * datos que encuentra en el archivo, si el personaje que logra leer es
 * valido (es del tipo entrenador, tiene nombre, tiene por lo menos un
 * pokemon), devuelve la direccion de este, en caso de no serlo, o
 * encontrar algun otro problema devuelve NULL
 */
void* archivo_2_personaje_principal (char ruta_archivo[]);

/*
 * Apartir de la ruta del archivo, carga el gimnasio con los datos que
 * encuentra en el archivo, si el gimnasio que logra leer es valido
 * (es del tipo gimnasio, tiene nombre, el primer entrenador es un lider,
 * y ese lider es valido), devuelve la direccion de este, en caso de no
 * serlo, o encontrar algun otro problema devuelve NULL
 */
void* archivo_2_gimnasio (char ruta_archivo[]);

/*
 * Agrega el gimnasio al mapa, que va a posicionarse de menor
 * dificultad a mayor
 */
int gimnasio_2_mapa(mapa_t* mapa, gimnasio_t* gimnasio);

/*
 * Dado un mapa, devuelve el gimnasio que tenga menor dificultad
 * y lo elimina del mapa. En caso de error devuelve NULL
 */
gimnasio_t* gimnasio_del_mapa(mapa_t* mapa);

/*
 * Dado un gimnasio, devuelve el entrenador que tenga este primero
 * en la lista, en caso de error devuelve NULL
 */
entrenador_t* entrenador_del_gimnasio(gimnasio_t* gimnasio);

/*
 * Dado un gimnasio, saca al entrenador que este en el primer
 * lugar
 */
void sacar_entrenador(gimnasio_t* gimnasio);

/*
 * Dado un gimnasio, saca al entrenador que este en el primer
 * lugar
 */
void sacar_gimnasio(mapa_t* mapa);

/*
 * Dado la posicion de un pokemon, devuelve el pokemon de la lista
 * de pokemones
 */
pokemon_t* elegir_pokemon(lista_t* pokemones, int posicion);

/*
 * Dados el personaje principal y entrenador o lider, se hace la
 * batalla, devolviendo 1 si el personaje principal gana o -1 si
 * gano el entrenador o lider
 */
int batalla_pokemon(entrenador_t* principal, entrenador_t* enemigo, funcion_batalla estilo);

/*
 * Dado un pokemon prestado del enemigo, se agrega a la lista de
 * pokemones del personaje pricipal, si tiene menos de
 * MAX_POKE_COMBATE (6) se agrega al final, si tiene mas se
 * agrega como septimo pokemon
 *
 * El id_pokemon va desde 0 siendo el primero pokemon hasta la
 * cantidad de pokemones del enemigo menos uno
 */
int tomar_prestado(entrenador_t* principal, entrenador_t* enemigo, int id_pokemon);

/*
 * Dado una lista de pokemones, intercambia los pokemones dado por
 * su id
 */
int reordenar_pokemones(lista_t* pokemones, int pkm_uno, int pkm_dos);

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el personaje, esto incluye todos sus pokemones
 */
void destruir_entrenador(void* entrenador);

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el gimnasio, esto incluye todos sus entrenadores que a su vez
 * tiene todos sus pokemones
 */
void destruir_gimnasio(void* gimnasio);

/*
 * Se encanrga de liberar toda la memoria necesaria que se utilizo al
 * crear el mapa
 */
void destruir_mapa(void* mapa);

#endif /* __HERRAMIENTAS_H__ */
