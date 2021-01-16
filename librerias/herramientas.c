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

#define NOMBRE_PENDIENTE "Nombre pendiente"
#define GANO_PRINCIPAL 1
#define GANO_ENEMIGO -1


typedef int (* leer_linea_archivo)(FILE*, void *);

int leer_pokemon(FILE* archivo, void* pokemon) {
    return fscanf(archivo, LEER_POKEMON,                                \
                  ((pokemon_t*)pokemon)->nombre,                        \
                  &(((pokemon_t*)pokemon)->velocidad),                  \
                  &(((pokemon_t*)pokemon)->ataque),                     \
                  &(((pokemon_t*)pokemon)->defensa));
}

int leer_gimnasio (FILE* archivo, void* gimnasio) {
    return fscanf(archivo, LEER_GIMNASIO,                               \
                  ((gimnasio_t*)gimnasio)->nombre,                      \
                  &(((gimnasio_t*)gimnasio)->dificultad),               \
                  &(((gimnasio_t*)gimnasio)->id_funcion));
}

int leer_entrenador (FILE* archivo, void* entrenador) {
    return fscanf(archivo, LEER_ENTRENADOR, (((entrenador_t*)entrenador)->nombre));
}

int leer_id (FILE* archivo, void* id) {
    return fscanf(archivo, LEER_ID, (char*)id);
}

bool entrenador_inicializado(entrenador_t* entrenador) {
    return (strcmp(((entrenador_t*)entrenador)->nombre, NOMBRE_PENDIENTE) != 0);
}

pokemon_t* crear_pokemon() {
    return calloc(1, sizeof(pokemon_t));
}

void destruir_pokemon(void* pokemon) {
    free(pokemon);
}

entrenador_t* crear_entrenador() {
    lista_liberar_elemento destructor = destruir_pokemon;
    entrenador_t entrenador;

    entrenador.cant_pokemones = 0;
    strcpy(entrenador.nombre, NOMBRE_PENDIENTE);
    entrenador.pokemones = lista_crear(destructor);
    entrenador.lider = false;

    if (!entrenador.pokemones) return NULL;

    entrenador_t* p_entrenador = calloc(1, sizeof(entrenador_t));

    if (!p_entrenador) {
        lista_destruir(entrenador.pokemones);
        return NULL;
    }

    (*p_entrenador) = entrenador;

    return p_entrenador;
}

void destruir_entrenador(void* entrenador) {
    if (!entrenador) return;

    lista_destruir(((entrenador_t*)entrenador)->pokemones);
    free(entrenador);
}

gimnasio_t* crear_gimnasio() {
    lista_liberar_elemento destructor = destruir_entrenador;
    gimnasio_t gimnasio;

    gimnasio.id_funcion = 0;
    gimnasio.dificultad = 0;
    gimnasio.cant_entrenadores = 0;
    gimnasio.entrenadores = lista_crear(destructor);

    if (!gimnasio.entrenadores) return NULL;

    gimnasio_t* p_gimnasio = calloc(1, sizeof(gimnasio_t));
    if (!p_gimnasio) {
        lista_destruir(gimnasio.entrenadores);
        return NULL;
    }

    (*p_gimnasio) = gimnasio;

    return p_gimnasio;
}

void destruir_gimnasio(void* gimnasio) {
    if (!gimnasio) return;

    lista_destruir(((gimnasio_t*)gimnasio)->entrenadores);
    free(gimnasio);
}

int comparador_gimnasios (void* elemento_uno, void* elemento_dos) {

    int dificultad_uno = ((gimnasio_t*)elemento_uno)->dificultad;
    int dificultad_dos = ((gimnasio_t*)elemento_dos)->dificultad;

    return dificultad_uno > dificultad_dos ? 1 : dificultad_uno < dificultad_dos ? -1 : 0;
}

mapa_t* crear_mapa() {
    heap_comparador comparador = comparador_gimnasios;
    heap_liberar_elemento destructor = destruir_gimnasio;
    mapa_t mapa;

    mapa.cant_gimnasios = 0;
    mapa.gimnasios = heap_crear(comparador, destructor);
    if (!mapa.gimnasios) return NULL;

    mapa_t* p_mapa = calloc(1, sizeof(mapa_t));
    if (!p_mapa) {
        heap_destruir(mapa.gimnasios);
        return NULL;
    }
    (*p_mapa) = mapa;

    return p_mapa;
}

void destruir_mapa(void* mapa) {
    if (!mapa) return;

    heap_destruir(((mapa_t*)mapa)->gimnasios);
    free(mapa);
}

int leer_linea(FILE* archivo, gimnasio_t* gimnasio, entrenador_t* entrenador, pokemon_t* pokemon, char* tipo) {
    int leido = leer_id(archivo, tipo);
    if (leido != CANT_ID)
        return leido;

    int cant_esperada;
    leer_linea_archivo leer_archivo;
    void* dato;

    if (*tipo == GIMNASIO) {
        cant_esperada = CANT_GIMNASIO;
        leer_archivo = leer_gimnasio;
        dato = gimnasio;
    } else if (*tipo == LIDER || *tipo == ENTRENADOR) {
        cant_esperada = CANT_ENTRENADOR;
        leer_archivo = leer_entrenador;
        dato = entrenador;
    } else if (*tipo == POKEMON) {
        cant_esperada = CANT_POKEMON;
        leer_archivo = leer_pokemon;
        dato = pokemon;
    } else {
        return leido;
    }

    leido = leer_archivo(archivo, dato);
    if (leido != cant_esperada)
        return leido;

    return leido;
}

int dato_esperado(FILE* archivo, gimnasio_t* gimnasio, entrenador_t* entrenador, pokemon_t* pokemon, char* tipo) {

    gimnasio_t gimnasio_aux;
    entrenador_t entrenador_aux;
    pokemon_t pokemon_aux;

    int resultado = leer_linea(archivo, &gimnasio_aux, &entrenador_aux, &pokemon_aux, tipo);
    if (resultado == EOF)
        return resultado;

    if (gimnasio && *tipo == GIMNASIO) {
        strcpy(gimnasio->nombre, gimnasio_aux.nombre);
        gimnasio->dificultad = gimnasio_aux.dificultad;
        gimnasio->id_funcion = gimnasio_aux.id_funcion;
    } else if (entrenador && (*tipo == LIDER || *tipo == ENTRENADOR)) {
        strcpy(entrenador->nombre, entrenador_aux.nombre);
        if (*tipo == LIDER)
            entrenador->lider = true;
        else
            entrenador->lider = false;
    } else if (pokemon && *tipo == POKEMON) {
        *pokemon = pokemon_aux;
    }

    return resultado;
}

int archivo_2_entrenador (FILE* archivo, void* entrenador, void* entrenador_siguiente, bool principal, bool lider) {
    entrenador_t siguiente;
    pokemon_t pokemon;
    bool guardar = true;
    int resultado;
    char tipo;

    if (!entrenador_inicializado(entrenador)) {
        resultado = dato_esperado(archivo, NULL, entrenador, NULL, &tipo);
        while (resultado != EOF && (tipo != LIDER && tipo != ENTRENADOR)) {
            resultado = dato_esperado(archivo, NULL, entrenador, NULL, &tipo);
        }

        if (resultado == EOF) return ERROR;
    }

    if (!((entrenador_t*)entrenador)->lider && lider)
        guardar = false;

    resultado = dato_esperado(archivo, NULL, &siguiente, &pokemon, &tipo);
    while (resultado != EOF && tipo == POKEMON) {
        if (guardar) {
            pokemon_t* p_pokemon = crear_pokemon();
            *p_pokemon = pokemon;

            resultado = lista_insertar(((entrenador_t*)entrenador)->pokemones, p_pokemon);
            if (resultado == ERROR)
                destruir_pokemon(p_pokemon);
        }

        if (((entrenador_t*)entrenador)->cant_pokemones >= MAX_POKEMON_COMBATE && !principal)
            guardar = false;

        resultado = dato_esperado(archivo, NULL, &siguiente, &pokemon, &tipo);
    }

    if (entrenador_siguiente && resultado != EOF) {
        strcpy(((entrenador_t*)entrenador_siguiente)->nombre, siguiente.nombre);
        ((entrenador_t*)entrenador_siguiente)->lider = siguiente.lider;
    }

    ((entrenador_t*)entrenador)->cant_pokemones = (int) ((entrenador_t*)entrenador)->pokemones->cantidad;

    return ((entrenador_t*)entrenador)->cant_pokemones;
}

int archivo_2_personaje_principal (char ruta_archivo[], void* principal) {
    if (!ruta_archivo || !principal)
        return ERROR;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return ERROR;

    int cant_pokemones = archivo_2_entrenador (archivo, ((entrenador_t*)principal), NULL, true, false);

    fclose(archivo);
    return (cant_pokemones > 0) ? EXITO : ERROR;
}

int archivo_2_gimnasio (char ruta_archivo[], void* gimnasio) {
    if (!ruta_archivo || !gimnasio)
        return ERROR;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return ERROR;

    char tipo;
    int resultado = dato_esperado(archivo, gimnasio, NULL, NULL, &tipo);
    if (resultado == EOF || tipo != GIMNASIO) {
        fclose(archivo);
        return ERROR;
    }

    entrenador_t* p_entrenador = crear_entrenador();
    entrenador_t entrenador_siguiente;
    bool lider = true, principal = false;
    strcpy(entrenador_siguiente.nombre, NOMBRE_PENDIENTE);

    int cant_pokemones = archivo_2_entrenador(archivo, p_entrenador, &entrenador_siguiente, principal, lider);
    if (cant_pokemones <= 0) {
        destruir_entrenador(p_entrenador);
        fclose(archivo);
        return ERROR;
    }

    lider = false;
    while (entrenador_inicializado(p_entrenador) && cant_pokemones >= 0) {
        if (cant_pokemones > 0) {
            resultado = lista_apilar(((gimnasio_t*)gimnasio)->entrenadores, p_entrenador);
            if (resultado == ERROR)
                destruir_entrenador(p_entrenador);
        } else {
            destruir_entrenador(p_entrenador);
        }

        p_entrenador = crear_entrenador();
        if (entrenador_inicializado(&entrenador_siguiente)) {
            strcpy(p_entrenador->nombre, entrenador_siguiente.nombre);
            p_entrenador->lider = entrenador_siguiente.lider;
        }
        strcpy(entrenador_siguiente.nombre, NOMBRE_PENDIENTE);

        cant_pokemones = archivo_2_entrenador(archivo, p_entrenador, &entrenador_siguiente, principal, lider);
    }

    ((gimnasio_t*)gimnasio)->cant_entrenadores = (int) ((gimnasio_t*)gimnasio)->entrenadores->cantidad;

    destruir_entrenador(p_entrenador);
    fclose(archivo);
    return (((gimnasio_t*)gimnasio)->cant_entrenadores > 0) ? EXITO : ERROR;
}

int gimnasio_2_mapa(mapa_t* mapa, gimnasio_t* gimnasio) {

    if (!mapa || !gimnasio) return ERROR;

    if (!mapa->gimnasios) return ERROR;

    int resultado = heap_insertar(mapa->gimnasios, gimnasio);

    if (resultado == ERROR) return ERROR;
    (mapa->cant_gimnasios)++;

    return EXITO;
}

gimnasio_t* gimnasio_del_mapa(mapa_t* mapa) {
    if (!mapa) return NULL;
    return heap_raiz(mapa->gimnasios);
}

entrenador_t* lider_del_gimnasio(gimnasio_t* gimnasio) {
    if (!gimnasio) return NULL;
    return lista_elemento_en_posicion(gimnasio->entrenadores, (size_t) gimnasio->cant_entrenadores - 1);
}

entrenador_t* entrenador_del_gimnasio(gimnasio_t* gimnasio) {
    if (!gimnasio) return NULL;
    return lista_elemento_en_posicion(gimnasio->entrenadores, 0);
}

void sacar_entrenador(gimnasio_t* gimnasio) {
    if (!gimnasio) return;
    lista_desapilar(gimnasio->entrenadores);
    gimnasio->cant_entrenadores = (int) gimnasio->entrenadores->cantidad;
}

void sacar_gimnasio(mapa_t* mapa) {
    if (!mapa) return;
    heap_eliminar_raiz(mapa->gimnasios);
    mapa->cant_gimnasios = (int) mapa->gimnasios->cant_elementos;
}

pokemon_t* pokemon_en_lista(lista_t* pokemones, int posicion) {
    if (!pokemones || posicion < 0) return NULL;
    return lista_elemento_en_posicion(pokemones, (size_t) posicion);
}

pokemon_t* elegir_pokemon(entrenador_t* principal, int posicion) {
    if (!principal) return NULL;
    return pokemon_en_lista(principal->pokemones, posicion);
}

/*
 * Dados dos pokemones y un tipo de pelea, devuelve 1 si gana
 * el primero, -1 si gana el segundo
 */
int pelea_pokemon(pokemon_t* pokemon_uno, pokemon_t* pokemon_dos, funcion_batalla estilo) {
    return estilo(pokemon_uno, pokemon_dos);
}

/*
 * Aumenta las caracteristicas del pokemon +1 en cada caracteristica
 * hasta llegar a 63
 */
int level_up(pokemon_t* pokemon) {
    if (!pokemon)
        return ERROR;

    (pokemon->velocidad) += ((pokemon->velocidad) <= MAX_LEVEL_UP) ? 1 : 0;
    (pokemon->defensa) += ((pokemon->defensa) <= MAX_LEVEL_UP) ? 1 : 0;
    (pokemon->ataque) += ((pokemon->ataque) <= MAX_LEVEL_UP) ? 1 : 0;

    return EXITO;
}

/*
 * Como solo pueden tener 6 pokemones para la pelea, si perdio mas
 * de 6 pokemones ya no puede pelear, pero si desde el principio
 * tenia menos pokemones, si llega a esa cantidad ya no puede pelear
 */
bool condicion_pelea (int pokemones_derrotados, int pokemones_para_pelea) {
    return (pokemones_derrotados < MAX_POKEMON_COMBATE && pokemones_derrotados < pokemones_para_pelea);
}

int batalla_pokemon(entrenador_t* principal, entrenador_t* enemigo, funcion_batalla estilo) {

    if (!principal || !enemigo || !estilo) return 0;

    int contador_principal = 0, contador_enemigo = 0;

    while (condicion_pelea(contador_principal, principal->cant_pokemones) \
           && condicion_pelea(contador_enemigo, enemigo->cant_pokemones)) {

        int resultado = pelea_pokemon(elegir_pokemon(principal, contador_principal), \
                                      elegir_pokemon(enemigo, contador_enemigo), \
                                      estilo);

        if (resultado == GANO_PRINCIPAL) {
            level_up(elegir_pokemon(principal, contador_principal));
            contador_enemigo++;
        } else {
            contador_principal++;
        }
    }

    return condicion_pelea(contador_principal, principal->cant_pokemones) ? GANO_PRINCIPAL : GANO_ENEMIGO;
}

int tomar_prestado(entrenador_t* principal, entrenador_t* enemigo, int id_pokemon) {
    if (!principal || !enemigo)
        return ERROR;

    pokemon_t* pokemon_prestado = elegir_pokemon(enemigo, id_pokemon);
    if (!pokemon_prestado)
        return ERROR;

    pokemon_t* pokemon = crear_pokemon();
    (*pokemon) = (*pokemon_prestado);

    int resultado = lista_insertar_en_posicion(principal->pokemones, pokemon, MAX_POKEMON_COMBATE);
    if (resultado == ERROR) {
        destruir_pokemon(pokemon);
        return ERROR;
    }

    (principal->cant_pokemones)++;
    return EXITO;
}

int reordenar_pokemones(lista_t* pokemones, int pkm_uno, int pkm_dos) {

    if (!pokemones)
        return ERROR;

    pokemon_t* pokemon_uno = pokemon_en_lista(pokemones, pkm_uno);
    if (!pokemon_uno)
        return ERROR;

    pokemon_t* pokemon_dos = pokemon_en_lista(pokemones, pkm_dos);
    if (!pokemon_dos)
        return ERROR;

    pokemon_t pokemon_aux = (*pokemon_uno);
    (*pokemon_uno) = (*pokemon_dos);
    (*pokemon_dos) = pokemon_aux;

    return EXITO;
}
