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
    if (!entrenador)
        return false;
    return (strcmp(entrenador->nombre, NOMBRE_PENDIENTE) != 0);
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

    entrenador.lider = false;
    strcpy(entrenador.nombre, NOMBRE_PENDIENTE);
    entrenador.pokemones = lista_crear(destructor);

    if (!entrenador.pokemones) return NULL;

    entrenador_t* p_entrenador = calloc(1, sizeof(entrenador_t));

    if (p_entrenador)
        (*p_entrenador) = entrenador;
    else
        lista_destruir(entrenador.pokemones);

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
    gimnasio.entrenadores = lista_crear(destructor);

    if (!gimnasio.entrenadores) return NULL;

    gimnasio_t* p_gimnasio = calloc(1, sizeof(gimnasio_t));

    if (p_gimnasio) 
        (*p_gimnasio) = gimnasio;
    else 
        lista_destruir(gimnasio.entrenadores);

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

    mapa.gimnasios = heap_crear(comparador, destructor);
    if (!mapa.gimnasios) return NULL;

    mapa_t* p_mapa = calloc(1, sizeof(mapa_t));

    if (p_mapa) 
        (*p_mapa) = mapa;
    else 
        heap_destruir(mapa.gimnasios);

    return p_mapa;
}

void destruir_mapa(void* mapa) {
    if (!mapa) return;

    heap_destruir(((mapa_t*)mapa)->gimnasios);
    free(mapa);
}

/*
 * Lee completamente una linea y dependiendo del dato que lee, lo guardara en el gimnasio,
 * entrenador, o pokemon, y devolvera cuando leyo
 */
int leer_linea(FILE* archivo, gimnasio_t* gimnasio, entrenador_t* entrenador, pokemon_t* pokemon, char* tipo) {
    int leido = leer_id(archivo, tipo);
    if (leido != CANT_ID)
        return leido;

    leer_linea_archivo leer_archivo;
    void* dato;

    if (*tipo == GIMNASIO) {
        leer_archivo = leer_gimnasio;
        dato = gimnasio;
    } else if (*tipo == LIDER || *tipo == ENTRENADOR) {
        leer_archivo = leer_entrenador;
        dato = entrenador;
    } else if (*tipo == POKEMON) {
        leer_archivo = leer_pokemon;
        dato = pokemon;
    } else {
        return leido;
    }

    return leer_archivo(archivo, dato);
}

/*
 * Como uno no puede saber que tipo de dato aparece en la siguiente linea hasta que
 * lee la linea, y si se lee la linea y no se guarda lo leido se pierde la informacion.
 * Esta funcion lee la linea y guarda lo que consigue, y dependiendo del valor que uno
 * espera que aparezca lo recibe o ignora, mediante pasar como parametro o no el dato
 * que uno quiere
 */
int dato_esperado(FILE* archivo, gimnasio_t* gimnasio, entrenador_t* entrenador, pokemon_t* pokemon, char* tipo) {

    gimnasio_t gimnasio_aux;
    entrenador_t entrenador_aux;
    pokemon_t pokemon_aux;

    int resultado = leer_linea(archivo, &gimnasio_aux, &entrenador_aux, &pokemon_aux, tipo);
    if (resultado == EOF)
        return EOF;

    if (gimnasio && *tipo == GIMNASIO) {
        strcpy(gimnasio->nombre, gimnasio_aux.nombre);
        gimnasio->dificultad = gimnasio_aux.dificultad;
        gimnasio->id_funcion = gimnasio_aux.id_funcion;
    } else if (entrenador && (*tipo == LIDER || *tipo == ENTRENADOR)) {
        strcpy(entrenador->nombre, entrenador_aux.nombre);
        entrenador->lider = (*tipo == LIDER) ? true : false;
    } else if (pokemon && *tipo == POKEMON) {
        *pokemon = pokemon_aux;
    }

    return resultado;
}

/*
 * Si el entrenador no tiene un nombre asignado se leera el archivo
 * ignorando las lineas hasta encontrar el formato de un entrenador
 * y de ahi leer el nombre
 */
int conseguir_nombre (FILE* archivo, entrenador_t* entrenador) {

    int resultado = 0;
    char tipo = NADA;

    resultado = dato_esperado(archivo, NULL, entrenador, NULL, &tipo);
    while (resultado != EOF && (tipo != LIDER && tipo != ENTRENADOR))
        resultado = dato_esperado(archivo, NULL, entrenador, NULL, &tipo);

    return (resultado != EOF) ? EXITO : ERROR;
}

/*
 * Agrega un pokemon a la lista de pokemones que tiene un entrenador
 */
void agregar_pokemon(void* entrenador, pokemon_t pokemon) {
    pokemon_t* p_pokemon = crear_pokemon();
    if (!p_pokemon) return;
    *p_pokemon = pokemon;

    int resultado = lista_insertar(((entrenador_t*)entrenador)->pokemones, p_pokemon);
    if (resultado == ERROR)
        destruir_pokemon(p_pokemon);
}


bool seguir_leyendo_pokemones (entrenador_t* entrenador, bool principal) {
    return (lista_elementos(entrenador->pokemones) < MAX_POKEMON_COMBATE || principal);
}

/*
 * Va leyendo el archivo y va agregando los pokemones a la lista del entrenador, en caso
 * de que lea un entrenador, devuelve (en el caso de que se le haya pasado por parametro
 * el proximo entrenador) el entrenador con su nombre y su categoria, en el caso de no
 * leer un pokemon o entrenador, dejara de leer 
 */
void guardar_pokemones (FILE* archivo, void* entrenador, void* entrenador_siguiente, bool principal) {
    
    if (entrenador_siguiente)
        strcpy(((entrenador_t*)entrenador_siguiente)->nombre, NOMBRE_PENDIENTE);
    pokemon_t pokemon;
    int resultado;
    char tipo;

    resultado = dato_esperado(archivo, NULL, entrenador_siguiente, &pokemon, &tipo);
    while (resultado != EOF && tipo == POKEMON && seguir_leyendo_pokemones(entrenador, principal)) {
        agregar_pokemon(entrenador, pokemon);
        resultado = dato_esperado(archivo, NULL, entrenador_siguiente, &pokemon, &tipo);
    }

}

/*
 * Lee el nombre del entrenador si todavia no tiene y los pokemones
 * siguiente, en el caso que se encuentra con un entrenador indicando
 * el final del entrenador principal, se guarda su nombre y su tipo
 *
 * Si el parametro principal no es true, y se encuentran mas de 6 pokemones
 * se leeran pero no se guardan
 *
 * Si se encuentra con un lider y se pasa como parametro que no es un lider
 * se leeran todos los pokemones pero no se guardaran
 */
entrenador_t* archivo_2_entrenador (FILE* archivo, entrenador_t* entrenador_siguiente, bool principal, bool lider) {
    entrenador_t* entrenador = crear_entrenador();

    if (entrenador_inicializado(entrenador_siguiente)) {
        strcpy(entrenador->nombre, entrenador_siguiente->nombre);
        entrenador->lider = entrenador_siguiente->lider;
        strcpy(entrenador_siguiente->nombre, NOMBRE_PENDIENTE);
    } else {
        int resultado = conseguir_nombre(archivo, entrenador);
        if (resultado == ERROR) {
            destruir_entrenador(entrenador);
            return NULL;
        }
    }

    if (entrenador->lider == lider)
        guardar_pokemones(archivo, entrenador, entrenador_siguiente, principal);

    if (lista_elementos(entrenador->pokemones) > 0)
        return entrenador;
    destruir_entrenador(entrenador);
    return NULL;
}

void* archivo_2_personaje_principal (char ruta_archivo[]) {
    if (!ruta_archivo)
        return NULL;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return NULL;

    entrenador_t* entrenador = archivo_2_entrenador (archivo, NULL, true, false);

    fclose(archivo);
    return entrenador;
}

/*
 * Agrega al entrenador a la pila de entrenadores del gimnasio
 * Devuelve EXITO si puede agregarlo, sino devuelve ERROR
 */
int agregar_entrenador(gimnasio_t* gimnasio, entrenador_t* entrenador) {
    if (!gimnasio || !entrenador)
        return ERROR;
    if (lista_elementos(entrenador->pokemones) > 0) {
        int resultado = lista_apilar(((gimnasio_t*)gimnasio)->entrenadores, entrenador);
        return (resultado != ERROR) ? EXITO : ERROR;
    }
    return ERROR;
}

/*
 * Va leyendo el archivo y va agregando los entrenadores validos al
 * gimnasio, donde el primer entrenador es un lider y el resto entrenadores.
 * Si el primer entrenador no es un lider devuelve ERROR, si un entrenador
 * tiene etiqueda de lider se ignora y no se agrega a la lista de entrenadores
 * Devuelve la cantidad de entrenadores que pudo guardar, si no puede
 * guardar nada manda ERROR
 */
void guardar_entrenadores (FILE* archivo, gimnasio_t* gimnasio) {

    int resultado;
    entrenador_t entrenador_siguiente;
    strcpy(entrenador_siguiente.nombre, NOMBRE_PENDIENTE);

    entrenador_t* p_entrenador = archivo_2_entrenador(archivo, &entrenador_siguiente, false, true);

    resultado = agregar_entrenador(gimnasio, p_entrenador);
    if (resultado == ERROR) {
        destruir_entrenador(p_entrenador);
        return;
    }

    while (entrenador_inicializado(&entrenador_siguiente)) {

        p_entrenador = archivo_2_entrenador(archivo, &entrenador_siguiente, false, false);
        if (p_entrenador)
            agregar_entrenador(gimnasio, p_entrenador);

    }
}

void* archivo_2_gimnasio (char ruta_archivo[]) {
    if (!ruta_archivo)
        return NULL;

    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) return NULL;

    char tipo;
    gimnasio_t* gimnasio = crear_gimnasio();

    dato_esperado(archivo, gimnasio, NULL, NULL, &tipo);
    if (tipo == GIMNASIO)
        guardar_entrenadores(archivo, gimnasio);

    fclose(archivo);
    if (lista_elementos(gimnasio->entrenadores) > 0)
        return gimnasio;
    destruir_gimnasio(gimnasio);
    return NULL;
}


int gimnasio_2_mapa(mapa_t* mapa, gimnasio_t* gimnasio) {

    if (!mapa || !gimnasio) return ERROR;

    if (!mapa->gimnasios) return ERROR;

    int resultado = heap_insertar(mapa->gimnasios, gimnasio);

    return (resultado == ERROR) ? ERROR : EXITO;
}

gimnasio_t* gimnasio_del_mapa(mapa_t* mapa) {
    if (!mapa) return NULL;
    return heap_raiz(mapa->gimnasios);
}

bool encontrar_lider (void* entrenador, void* aux) {
    if (((entrenador_t*)entrenador)->lider) {
        aux = entrenador;
        return false;
    }
    return true;
}

entrenador_t* lider_del_gimnasio(gimnasio_t* gimnasio) {
    if (!gimnasio) return NULL;
    bool (*funcion_encontrar)(void*, void*) = encontrar_lider;
    entrenador_t* entrenador = NULL;
    lista_con_cada_elemento(gimnasio->entrenadores, funcion_encontrar, entrenador);
    return entrenador;
}

bool encontrar_entrenador (void* entrenador, void* aux) {
    aux = entrenador;
    return false;
}

entrenador_t* entrenador_del_gimnasio(gimnasio_t* gimnasio) {
    if (!gimnasio) return NULL;
    bool (*funcion_encontrar)(void*, void*) = encontrar_entrenador;
    entrenador_t* entrenador = NULL;
    lista_con_cada_elemento(gimnasio->entrenadores, funcion_encontrar, entrenador);
    return entrenador;
}

void sacar_entrenador(gimnasio_t* gimnasio) {
    if (!gimnasio) return;
    lista_desapilar(gimnasio->entrenadores);
}

void sacar_gimnasio(mapa_t* mapa) {
    if (!mapa) return;
    heap_eliminar_raiz(mapa->gimnasios);
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
bool condicion_pelea (size_t pokemones_derrotados, size_t pokemones_para_pelea) {
    return (pokemones_derrotados < MAX_POKEMON_COMBATE && pokemones_derrotados < pokemones_para_pelea);
}

int batalla_pokemon(entrenador_t* principal, entrenador_t* enemigo, funcion_batalla estilo) {

    if (!principal || !enemigo || !estilo) return 0;

    size_t contador_principal = 0, contador_enemigo = 0;

    while (condicion_pelea(contador_principal, lista_elementos(principal->pokemones)) \
           && condicion_pelea(contador_enemigo, lista_elementos(enemigo->pokemones))) {

        int resultado = pelea_pokemon(elegir_pokemon(principal, (int) contador_principal), \
                                      elegir_pokemon(enemigo, (int) contador_enemigo), \
                                      estilo);

        if (resultado == GANO_PRINCIPAL) {
            level_up(elegir_pokemon(principal, (int) contador_principal));
            contador_enemigo++;
        } else {
            contador_principal++;
        }
    }

    return condicion_pelea(contador_principal, lista_elementos(principal->pokemones)) ? GANO_PRINCIPAL : GANO_ENEMIGO;
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

    lista_borrar_de_posicion(enemigo->pokemones, (size_t) id_pokemon);
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
