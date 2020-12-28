#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "herramientas.h"

#define LEER_POKEMON "%80[^;];%i;%i;%i\n;"
#define CANT_POKEMON 4
#define LEER_GIMNASIO "%80[^;];%i;%i\n"
#define CANT_GIMNASIO 3
#define LEER_ENTRENADOR "%80[^\n]\n"
#define CANT_ENTRENADOR 1
#define LEER_ID "%c;"
#define CANT_ID 1

int leer_id (FILE* archivo, char* id) {
    return fscanf(archivo, LEER_ID, id);
}

int leer_pokemon(FILE* archivo, pokemon_t* pokemon) {
    return fscanf(archivo, LEER_POKEMON, pokemon->nombre, &(pokemon->velocidad), &(pokemon->defensa), &(pokemon->ataque));
}

int leer_gimnasio (FILE* archivo, gimnasio_t* gimnasio) {
    return fscanf(archivo, LEER_GIMNASIO, gimnasio->nombre, &(gimnasio->dificultad), &(gimnasio->id_funcion));
}

int leer_entrenador (FILE* archivo, char nombre[]) {

    char id;
    int leido = leer_id(archivo, &id);
    if (leido != CANT_ID || id != ENTRENADOR) return ERROR;

    leido = fscanf(archivo, LEER_ENTRENADOR, nombre);
    if (leido != CANT_ENTRENADOR) return ERROR;

    return EXITO;
}

pokemon_t* pokemon_crear() {
    return calloc(1, sizeof(pokemon_t));
}

int guardar_pokemones(FILE* archivo, lista_t* pokemones) {
    int cant_pokemones = 0, resultado;
    pokemon_t* p_pokemon;
    char id;

    int leido = leer_id(archivo, &id);
    while (leido == CANT_ID || id == POKEMON) {

        p_pokemon = pokemon_crear();
        if (!p_pokemon) return ERROR;

        leido = leer_pokemon(archivo, p_pokemon);
        if (leido != CANT_POKEMON) {
            free(p_pokemon);
            return ERROR;
        }

        resultado = lista_insertar(pokemones, p_pokemon);
        if (resultado == ERROR)  {
            free(p_pokemon);
            return ERROR;
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

    int resultado = leer_entrenador(archivo, principal->nombre);
    if (resultado == ERROR) return ERROR;

    principal->pokemones = lista_crear();
    resultado = guardar_pokemones(archivo, principal->pokemones);

    if (resultado == ERROR) {
        lista_destruir(principal->pokemones);
        return ERROR;
    }

    principal->cant_pokemones = resultado;
    return EXITO;
}
