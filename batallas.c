#include <string.h>

#include "batallas.h"
#include "herramientas.h"

int funcion_batalla_1(void* pkm_1, void* pkm_2) {

    int ataque_1 = ((pokemon_t*)pkm_1)->ataque;
    int ataque_2 = ((pokemon_t*)pkm_2)->ataque;

    return (ataque_1 > ataque_2) ? GANO_PRIMERO : GANO_SEGUNDO;
}

int funcion_batalla_2(void* pkm_1, void* pkm_2){

    int ataque_1 = ((pokemon_t*)pkm_1)->ataque * ((pokemon_t*)pkm_1)->velocidad;
    int ataque_2 = ((pokemon_t*)pkm_2)->ataque * ((pokemon_t*)pkm_2)->velocidad;

    return (ataque_1 > ataque_2) ? GANO_PRIMERO : GANO_SEGUNDO;
}

int funcion_batalla_3(void* pkm_1, void* pkm_2){

    int ataque_1 = ((pokemon_t*)pkm_1)->ataque - ((pokemon_t*)pkm_2)->defensa;
    int ataque_2 = ((pokemon_t*)pkm_2)->ataque - ((pokemon_t*)pkm_1)->defensa;

    return (ataque_1 > ataque_2) ? GANO_PRIMERO : GANO_SEGUNDO;
}

int funcion_batalla_4(void* pkm_1, void* pkm_2){

    int modificador_1 = ((pokemon_t*)pkm_1)->velocidad / ((pokemon_t*)pkm_1)->defensa;
    int modificador_2 = ((pokemon_t*)pkm_2)->velocidad / ((pokemon_t*)pkm_2)->defensa;

    int ataque_1 = ((pokemon_t*)pkm_1)->ataque * modificador_1;
    int ataque_2 = ((pokemon_t*)pkm_2)->ataque * modificador_2;

    return (ataque_1 > ataque_2) ? GANO_PRIMERO : GANO_SEGUNDO;
}

int funcion_batalla_5(void* pkm_1, void* pkm_2){

    // la velocidad se ve disminuida por lo que tardan en decir su nombre
    int modificador_1 = ((pokemon_t*)pkm_1)->velocidad / (int) strlen(((pokemon_t*)pkm_1)->nombre);
    int modificador_2 = ((pokemon_t*)pkm_2)->velocidad / (int) strlen(((pokemon_t*)pkm_2)->nombre);

    int ataque_1 = ((pokemon_t*)pkm_1)->ataque * modificador_1;
    int ataque_2 = ((pokemon_t*)pkm_2)->ataque * modificador_2;

    return (ataque_1 > ataque_2) ? GANO_PRIMERO : GANO_SEGUNDO;
}
