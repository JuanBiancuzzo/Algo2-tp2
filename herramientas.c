#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "herramientas.h"

#define LEER_POKEMON "%80[^;];%i;%i;%i\n"
#define CANT_POKEMON 4
#define LEER_GIMNASIO "%80[^;];%i;%i\n"
#define CANT_GIMNASIO 3
#define LEER_ENTRENADOR "%80[^\n]\n"
#define CANT_ENTRENADOR 1
#define LEER_ID "%c;"
#define CANT_ID 1
#define NADA 'N'

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

/*
 * Va leyendo un archivo y va guardandolos en la cola
 */
int guardar_pokemon (FILE* archivo, lista_t* pokemones, int* cantidad, char* ultima_leida) {

    char id_resultado;
    pokemon_t pokemon;

    int leido = leer_id(archivo, &id_resultado);
    if (leido != CANT_ID || id_resultado != POKEMON) {
        if (leido == CANT_ID)
            (*ultima_leida) = id_resultado;
        return ERROR;
    }

    leido = leer_pokemon(archivo, &pokemon);
    if (leido != CANT_POKEMON)
        return ERROR;

    pokemon_t* p_pokemon = calloc(1, sizeof(pokemon_t));
    if (!p_pokemon) return ERROR;

    (*p_pokemon) = pokemon;

    int resultado = lista_encolar(pokemones, p_pokemon);
    if (resultado == ERROR)  {
        free(p_pokemon);
        return ERROR;
    }

    (*cantidad)++;
    return EXITO;
}

/*
 * Va leyendo un archivo y va guardandolos en la cola
 */
int guardar_pokemones(FILE* archivo, lista_t* pokemones, char* ultima_leida) {
    int cant_pokemones = 0;

    int resultado = guardar_pokemon(archivo, pokemones, &cant_pokemones, ultima_leida);

    while (resultado == EXITO)
        resultado = guardar_pokemon(archivo, pokemones, &cant_pokemones, ultima_leida);

    return cant_pokemones;
}

/*
 * Guarda el tipo de entrenador (ya sea lider o entrenador) en una
 * pila, y guarda sus pokemones
 */
int guardar_entrenador(FILE* archivo, char id, lista_t* entrenadores, int* cantidad, char* ultima_leida) {
    entrenador_t entrenador;
    char id_resultado;
    int leido;

    if (*(ultima_leida) == NADA) {

        leido = leer_id(archivo, &id_resultado);
        if (leido != CANT_ID || id_resultado != id)
            return ERROR;

    } else {
        if (*(ultima_leida) != ENTRENADOR)
            return ERROR;
        else
            *(ultima_leida) = NADA;
    }

    entrenador.cant_pokemones = 0;
    leido = leer_entrenador(archivo, entrenador.nombre);
    if (leido != CANT_ENTRENADOR)
        return ERROR;

    entrenador.pokemones = lista_crear();
    int resultado = guardar_pokemones(archivo, entrenador.pokemones, ultima_leida);

    if (resultado == 0) {
        lista_destruir(entrenador.pokemones);
        return ERROR;
    }
    entrenador.cant_pokemones = resultado;

    entrenador_t* p_entrenador = calloc(1, sizeof(entrenador_t));
    if (!p_entrenador) return ERROR;

    (*p_entrenador) = entrenador;

    resultado = lista_apilar(entrenadores, p_entrenador);
    if (resultado == ERROR)  {
        free(p_entrenador);
        return ERROR;
    }

    (*cantidad)++;
    return EXITO;
}

/*
 * Va leyendo un archivo y va guardandolos en la pila
 */
int guardar_entrenadores(FILE* archivo, lista_t* entrenadores, char* ultima_leida) {
    int cant_entrenadores = 0;

    int resultado = guardar_entrenador(archivo, LIDER, entrenadores, &cant_entrenadores, ultima_leida);

    while (resultado == EXITO)
        resultado = guardar_entrenador(archivo, ENTRENADOR, entrenadores, &cant_entrenadores, ultima_leida);

    return cant_entrenadores;
}

int archivo_2_personaje_principal (char ruta_archivo[], personaje_t* principal) {
    if (!ruta_archivo || !principal)
        return ERROR;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return ERROR;

    char id, ultima_leida = NADA;
    int leido = leer_id(archivo, &id);
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
    int resultado = guardar_pokemones(archivo, principal->pokemones, &ultima_leida);

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

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return ERROR;

    char id, ultima_leida = NADA;
    int leido = leer_id(archivo, &id);
    if (leido != CANT_ID || id != GIMNASIO) {
        fclose(archivo);
        return ERROR;
    }

    leido = leer_gimnasio(archivo, gimnasio);
    if (leido != CANT_GIMNASIO) {
        fclose(archivo);
        return ERROR;
    }

    gimnasio->entrenadores = lista_crear();
    int resultado = guardar_entrenadores(archivo, gimnasio->entrenadores, &ultima_leida);

    if (resultado == 0) {
        lista_destruir(gimnasio->entrenadores);
        fclose(archivo);
        return ERROR;
    }
    gimnasio->cant_entrenadores = resultado;

    fclose(archivo);
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
    if (((entrenador_t*)entrenador)->cant_pokemones > 0)
        destruir_cola_pokemones(((entrenador_t*)entrenador)->pokemones);

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
