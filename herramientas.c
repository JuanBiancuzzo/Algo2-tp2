#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "herramientas.h"

#define LEER_POKEMON "%80[^;];%i;%i;%i\n;"
#define CANT_POKEMON 4
#define LEER_GIMNASIO "%80[^;];%i;%i\n"
#define CANT_GIMNASIO 3
#define LEER_ENTRENADOR "%80[^\n]\n"
#define CANT_ENTRENADOR 1
#define LEER_ID "%c;"
#define CANT_ID 1

int leer_pokemon(FILE* archivo, pokemon_t* pokemon) {
    return fscanf(archivo, LEER_POKEMON, pokemon->nombre, &(pokemon->velocidad), &(pokemon->defensa), &(pokemon->ataque));
}

int leer_gimnasio (FILE* archivo, gimnasio_t* gimnasio) {
    return fscanf(archivo, LEER_GIMNASIO, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio->id_funcion));
}

int leer_entrenador (FILE* archivo, char nombre[]) {
    return fscanf(archivo, LEER_ENTRENADOR, nombre);
}

int leer_id (FILE* archivo, char* id) {
    return fscanf(archivo, LEER_ID, id);
}

int guardar_pokemones(FILE* archivo, lista_t* pokemones) {
    int cant_pokemones = 0, resultado;
    pokemon_t* p_pokemon;
    char id;

    int leido = leer_id(archivo, &id);
    while (leido == CANT_ID && id == POKEMON) {

        p_pokemon = calloc(1, sizeof(pokemon_t));
        if (!p_pokemon) return cant_pokemones;

        leido = leer_pokemon(archivo, p_pokemon);
        if (leido != CANT_POKEMON) {
            free(p_pokemon);
            return cant_pokemones;
        }

        resultado = lista_insertar(pokemones, p_pokemon);
        if (resultado == ERROR)  {
            free(p_pokemon);
            return cant_pokemones;
        }

        cant_pokemones++;
        leido = leer_id(archivo, &id);
    }
    return cant_pokemones;
}

int archivo_2_personaje_principal (char ruta_archivo[], personaje_t* principal) {
    if (!ruta_archivo || !principal)
        return ERROR;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return ERROR;

    char id;
    int leido = leer_id(archivo, &id), resultado;

    if (leido != CANT_ID || id != ENTRENADOR) {
        fclose(archivo);
        return ERROR;
    }

    leido = leer_entrenador(archivo, principal->nombre);
    if (leido != CANT_ENTRENADOR) {
        fclose(archivo);
        return ERROR;
    }

    principal->pokemones = lista_crear();
    resultado = guardar_pokemones(archivo, principal->pokemones);

    if (resultado == 0) {
        lista_destruir(principal->pokemones);
        fclose(archivo);
        return ERROR;
    }

    principal->cant_pokemones = resultado;
    fclose(archivo);
    return EXITO;
}

int archivo_2_gimnasio (char ruta_archivo[], gimnasio_t* gimnasio) {
    if (!ruta_archivo || !gimnasio)
        return ERROR;

    return EXITO;
}

personaje_t* crear_personaje_principal () {
    personaje_t* personaje = calloc(1, sizeof(personaje_t));

    if (!personaje) return NULL;

    personaje->cant_pokemones = 0;
    return personaje;
}

gimnasio_t* crear_gimnasio () {
    gimnasio_t* gimnasio = calloc(1, sizeof(gimnasio_t));

    if (!gimnasio) return NULL;

    gimnasio->cant_entrenadores = 0;
    return gimnasio;
}

/*
 * Funcion que se encarga de liberar el puntero en el heap
 * de cada pokemon
 */
bool liberar_pokemones(void* pokemon, void* contexto) {
    contexto = contexto;
    free(pokemon);
    return true;
}

/*
 * Se encarga de liberar todos los pokemones en la cola
 */
void destruir_cola_pokemones(lista_t* pokemones) {
    bool (*funcion) (void*, void*) = liberar_pokemones;
    lista_con_cada_elemento(pokemones, funcion, NULL);
    lista_destruir(pokemones);
}

void destruir_personaje_principal(personaje_t* principal) {

    if (!principal) return;

    if (principal->cant_pokemones > 0)
        destruir_cola_pokemones(principal->pokemones);

    free(principal);
}

/*
 * Funcion que se encarga de liberar a todos los pokemones que tengan
 * y despues libere al entrenador
 */
bool liberar_entrenadores(void* entrenador, void* contexto) {
    contexto = contexto;
    if (((entrenador_t*)entrenador)->cant_pokemones > 0) {
        destruir_cola_pokemones(((entrenador_t*)entrenador)->pokemones);
        lista_destruir(((entrenador_t*)entrenador)->pokemones);
    }
    free(entrenador);
    return true;
}

/*
 * Se encarga de liberar todos los entrenadores en la pila
 */
void destruir_pila_entrenadores(lista_t* entrenadores) {
    bool (*funcion) (void*, void*) = liberar_entrenadores;
    lista_con_cada_elemento(entrenadores, funcion, NULL);
    lista_destruir(entrenadores);
}

void destruir_gimnasio(gimnasio_t* gimnasio) {

    if (!gimnasio) return;

    if (gimnasio->cant_entrenadores > 0)
        destruir_pila_entrenadores(gimnasio->entrenadores);

    free(gimnasio);
}
