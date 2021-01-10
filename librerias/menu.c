#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

#ifdef __linux__
#include <unistd.h>
#define CLEAR system("clear")
#define SLEEP sleep(1)
#elif _WIN32
#include <windows.h>
#define CLEAR system("cls")
#define SLEEP Sleep(100)
#endif

#define FONDO_BLANCO "\033[0;47;32m"
#define NORMAL "\033[0m"
#define MAX_ANCHO 200
#define MAX_ALTO 200
#define MAX_LINEAS 10

#define ANCHO_POKEMON 20
#define ALTO_POKEMON 4

typedef struct coordenadas {
    int x;
    int y;
} coor_t;

typedef struct pantalla {
    char display[MAX_ANCHO][MAX_ALTO];
    int ancho;
    int alto;
} pantalla_t;

int minimo (int primero, int segundo) {
    return (primero < segundo) ? primero : segundo;
}

/*
 * Crea, por terminar, una linea de caracteres con
 * fondo blanco y un ancho especifico
 */
void linea_blanca (int ancho) {
    printf(FONDO_BLANCO);
    for (int i = 0; i < ancho; i++)
        printf(" ");
    printf(NORMAL);
}

/*
 * Dado un string, y un espacio, devuelve la posicion
 * en la que el string tiene que empezar para que se
 * vea centrado en ese espacio
 */
int posicion_media (char* texto, int ancho) {
    return (ancho/2) - (int)(strlen(texto)/2);
}

/*
 * Sorpresa, sorpresa, inicializa una matriz con
 * espacios
 */
void inicializar_matriz(pantalla_t* pantalla){
    for (int i = 0; i < pantalla->alto; i++)
        for (int j = 0; j < pantalla->ancho; j++)
            pantalla->display[i][j] = ' ';
}

/*
 * Dado una pantalla, los imprime con un borde
 * blanco
 */
void imprimir_pantalla (pantalla_t pantalla) {
    linea_blanca(pantalla.ancho + 4);
    printf("\n");
    for (int i = 0; i < pantalla.alto; i++) {
        linea_blanca(2);
        int largo = minimo((int) strlen(pantalla.display[i]), pantalla.ancho);

        for (int j = 0; j < largo; j++)
            printf("%c", pantalla.display[i][j]);

        for (int j = (int) strlen (pantalla.display[i]); j < pantalla.ancho ; j++)
            printf(" ");

        linea_blanca(2);
        printf("\n");
    }
    linea_blanca(pantalla.ancho + 4);
    printf("\n");
}

/*
 * Inserta una texto en la pantalla, en una pocicion x e y
 */
void texto_2_pantalla (pantalla_t* pantalla, char* texto, int largo, coor_t coor) {
    for (int i = 0; i < largo; i++)
        (pantalla->display)[coor.x][coor.y + i] = texto[i];
}

/*
 * Inserta el bloque de textos en la pantalla en
 * las posiciones x e y
 */
void bloque_2_pantalla (pantalla_t* pantalla, pantalla_t bloque, coor_t coor) {
    for (int i = 0; i < bloque.alto; i++) {
        texto_2_pantalla(pantalla, bloque.display[i], bloque.ancho, coor);
        (coor.x)++;
    }
}

/*
 * Dado una serie de instrucciones, devuelve una
 * bloque con los elementos centrados en este
 */
void crear_instruccion (pantalla_t* bloque, \
                        char letra, char* instruc[MAX_INSTRUC], int tope_instrucciones, \
                        int espacio)  {

    char caracter[4] = {'[', letra, ']'};
    coor_t coor = {0, posicion_media(caracter, espacio)};
    texto_2_pantalla(bloque, caracter, 3, coor);

    for (int i = 0; i < tope_instrucciones; i++) {
        coor.x = 1 + i;
        coor.y = posicion_media(instruc[i], espacio);
        texto_2_pantalla(bloque, instruc[i], (int) strlen(instruc[i]), coor);
    }
}

void crear_titulo(pantalla_t* bloque, char* texto, int ancho) {
    coor_t coor = {0, posicion_media(texto, ancho)};
    texto_2_pantalla(bloque, texto, (int) strlen(texto), coor);
}

/*
 * Si ancho o alto es menor a 0, se mantiene el ancho y alto,
 * respectivamente, de la pantalla
 */
pantalla_t cambiar_pantalla(pantalla_t pantalla, int ancho, int alto) {
    if (ancho >= 0) pantalla.ancho = ancho;
    if (alto >= 0) pantalla.alto = alto;
    return pantalla;
}

void menu_inicio(char* instrucciones) {

    int cant_instrucciones = 4, cant_lineas = 3, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 6;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[5] = {INGRESAR_ARCHIVO, AGREGAR_GIMNASIO, COMENZAR_PARTIDA, SIMULAR_PARTIDA};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Archivo", "del personaje" , "principal"}, \
                                            {"Agregar" , "archivo" , "del gimnasio"}, \
                                            {"Comenzar", "partida" , " "}, \
                                            {"Simular" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_gimnasio(char instrucciones[]) {

    int cant_instrucciones = 4, cant_lineas = 3, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 6;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[5] = {MOSTRAR_ENTRENADOR, MOSTRAR_GIMNASIO, CAMBIAR_POKEMON, PROXIMA_BATALLA};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Mostrar el", "entrenador" , " "}, \
                                            {"Mostrar el" , "gimnasio" , " "}, \
                                            {"Cambiar el", "orden de tus" , "pokemones"}, \
                                            {"Proxima" , "batalla" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_batalla(char instrucciones[]) {

    int cant_instrucciones = 1, cant_lineas = 2, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 5;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[2] = {PROXIMO_COMBATE};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Proximo", "combate"}};

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_victoria(char instrucciones[]) {

    int cant_instrucciones = 3, cant_lineas = 3, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 6;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[4] = {TOMAR_PRESTADO, CAMBIAR_POKEMONES, PROXIMO_GIMNASIO};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Tomar prestado", "un pokemon"   , "del lider"}, \
                                            {"Cambiar el"    , "orden de tus" , "pokemones"}, \
                                            {"Proximo"       , "gimnasio"     , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_derrota(char instrucciones[]) {

    int cant_instrucciones = 3, cant_lineas = 3, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 6;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[4] = {CAMBIAR_POKEMONES, REINTENTAR, FINALIZAR};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Cambiar el", "orden de tus" , "pokemones"}, \
                                            {"Reintentar", "gimnasio" , " "}, \
                                            {"Finalizar" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_confirmacion(char instrucciones[], char* frase) {

    int cant_instrucciones = 2, cant_lineas = 1, linea_inicial = 2;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO, pantalla.alto = 5;
    bloque.ancho = pantalla.ancho, bloque.alto = pantalla.alto;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    crear_titulo(&bloque, frase, bloque.ancho);
    coor_t coor = {1, 0};

    bloque_2_pantalla(&pantalla, cambiar_pantalla(bloque, -1, 3), coor);
    inicializar_matriz(&bloque);

    bloque.ancho = pantalla.ancho / cant_instrucciones, bloque.alto = pantalla.alto - 2;
    char instruc[3] = {AFIRMAR, NEGAR};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Afirmar"}, \
                                            {"Negar"} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void menu_avanzar_retroceder(char instrucciones[]) {

    int cant_instrucciones = 2, cant_lineas = 1, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 4;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    char instruc[3] = {SIGUIENTE, ANTERIOR};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Siguiente"}, \
                                            {"Anterior"} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(&bloque, instruc[i], items[i], cant_lineas, bloque.ancho);
        coor_t coor = {linea_inicial, bloque.ancho * i};
        bloque_2_pantalla(&pantalla, bloque, coor);
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

int potencia (int base, int potencia) {
    int resultado = 1;
    for (int i = 0; i < potencia; i++)
        resultado *= base;
    return resultado;
}

void numero_2_texto(char texto[], int numero, int cant_numeros) {
    for (int i = 0; i < cant_numeros; i++)
        texto[cant_numeros - (1 + i)] = (char) (48 + (numero % potencia(10, i+1)) / potencia(10, i));
}

void bloque_estadistica(pantalla_t* bloque, char letra, int valor, coor_t desfase) {
    char frase[6] = {letra, ':'};
    numero_2_texto(frase+2, valor, 2);
    texto_2_pantalla(bloque, frase, (int)strlen(frase), desfase);
}

void poner_esquinas (pantalla_t* pantalla) {
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            pantalla->display[(pantalla->alto - 1) * i][(pantalla->ancho - 1) * j] = '+';
}

void poner_lado_vertical(pantalla_t* pantalla, int altura) {
    for (int i = 1; i < pantalla->alto - 1; i++)
        pantalla->display[i][altura] = '|';
}

void poner_lado_horizontal(pantalla_t* pantalla, int altura) {
    for (int i = 1; i < pantalla->ancho - 1; i++)
        pantalla->display[altura][i] = '-';
}

void poner_cuadrado(pantalla_t* pantalla) {
    poner_esquinas(pantalla);

    poner_lado_horizontal(pantalla, 0);
    poner_lado_horizontal(pantalla, pantalla->alto - 1);

    poner_lado_vertical(pantalla, 0);
    poner_lado_vertical(pantalla, pantalla->ancho - 1);
}

void bloque_pokemon (pantalla_t* bloque, pokemon_t pokemon) {

    poner_cuadrado(bloque);

    int largo = minimo((int) strlen(pokemon.nombre), bloque->ancho - 2);
    coor_t coor = {1, posicion_media(pokemon.nombre, bloque->ancho)};
    texto_2_pantalla(bloque, pokemon.nombre, largo, coor);

    coor_t desfase = {2, 2};
    bloque_estadistica(bloque, 'a', pokemon.ataque, desfase);
    desfase.y = 8;
    bloque_estadistica(bloque, 'd', pokemon.defensa, desfase);
    desfase.y = 14;
    bloque_estadistica(bloque, 'v', pokemon.velocidad, desfase);
}

void mostrar_intercambiar_pokemones(pokemon_t* pkm1, pokemon_t* pkm2, char* frase) {

    int cant_pokemones = 2 , ancho_pkm = 20, linea_inicial = 2;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO, pantalla.alto = 7;
    bloque.ancho = pantalla.ancho, bloque.alto = pantalla.alto;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    crear_titulo(&bloque, frase, bloque.ancho);
    coor_t coor = {1, 0};

    bloque_2_pantalla(&pantalla, cambiar_pantalla(bloque, -1, 3), coor);
    inicializar_matriz(&bloque);

    bloque.ancho = pantalla.ancho / cant_pokemones, bloque.alto = pantalla.alto - 3;
    pokemon_t* pokemones[2] = {pkm1, pkm2};

    for (int i = 0; i < cant_pokemones; i++) {
        bloque.ancho = ancho_pkm;

        pokemon_t pkm_vacio = {"Elegir pokemon", 0, 0, 0};
        if (pokemones[i]) pkm_vacio = *pokemones[i];

        bloque_pokemon (&bloque, pkm_vacio);

        int posicion_media = (pantalla.ancho / cant_pokemones) / 2 - ancho_pkm / 2;
        coor_t coor = {linea_inicial, posicion_media + (pantalla.ancho / cant_pokemones) * i};
        bloque_2_pantalla(&pantalla, bloque, coor);

        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
}

void mostrar_pila_pokemones (pantalla_t* pantalla, lista_t pokemones, int cant_pokemones, int maximo, int desfase) {

    pantalla_t bloque;
    bloque.ancho = ANCHO_POKEMON, bloque.alto = ALTO_POKEMON;
    inicializar_matriz(&bloque);

    maximo += desfase;
    int iteraciones = minimo(cant_pokemones, maximo);

    for (int i = desfase; i < iteraciones; i++) {
        pokemon_t* pokemon = (pokemon_t*)lista_elemento_en_posicion(&pokemones, (size_t) i);
        if (pokemon) {
            bloque_pokemon(&bloque, *pokemon);
            coor_t coor = {bloque.alto * (i - desfase), 0};
            bloque_2_pantalla(pantalla, bloque, coor);
        }
        inicializar_matriz(&bloque);
    }
}

int mostrar_pokemones(lista_t pokemones, int cant_pokemones) {

    int ancho_pkm = ANCHO_POKEMON, maximo = 6;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO, pantalla.alto = ALTO;
    bloque.ancho = ancho_pkm, bloque.alto = ALTO;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    mostrar_pila_pokemones(&bloque, pokemones, cant_pokemones, maximo, 0);

    coor_t desfase = {2, 3};
    bloque_2_pantalla(&pantalla, bloque, desfase);
    inicializar_matriz(&bloque);

    int separacion = desfase.y * 2 + ancho_pkm;
    poner_lado_vertical(&pantalla, separacion);

    int limite = minimo(cant_pokemones/maximo, 4);
    desfase.y = separacion + 4;

    for (int i = 0; i < limite; i++) {
        inicializar_matriz(&bloque);
        mostrar_pila_pokemones(&bloque, pokemones, cant_pokemones, maximo, maximo * (1+i));
        bloque_2_pantalla(&pantalla, bloque, desfase);
        desfase.y += ancho_pkm + 3;
    }

    imprimir_pantalla(pantalla);
    return 0;
}

void mostrar_columna_entrandor (pantalla_t* pantalla, entrenador_t entrenador, bool lider) {

    pantalla_t bloque;
    bloque.ancho = pantalla->ancho, bloque.alto = pantalla->alto;
    inicializar_matriz(&bloque);

    crear_titulo(pantalla, entrenador.nombre, pantalla->ancho);

    if (lider) {
        coor_t coor = {1, 0};
        crear_titulo(&bloque, "(Lider)", pantalla->ancho);
        bloque_2_pantalla(pantalla, cambiar_pantalla(bloque, pantalla->ancho, 1), coor);
        inicializar_matriz(&bloque);
    }

    mostrar_pila_pokemones(&bloque, *(entrenador.pokemones), entrenador.cant_pokemones, 6, 0);
    coor_t coor = {2, 0};
    bloque_2_pantalla(pantalla, bloque, coor);
}

void circulo (pantalla_t* bloque, int radio, coor_t posicion, int rango) {

    int relacion = 2, centro = bloque->ancho / 4;
    int desfase = centro - radio;

    for (int i = -rango; i < rango; i++)
        if (potencia(posicion.x - radio, 2) + potencia(posicion.y - radio, 2) == potencia(radio, 2)+i)
            bloque->display[posicion.x + desfase][(posicion.y + desfase) * relacion] = 'o';
}

void pokebola (pantalla_t* pantalla) {

    int radio = (pantalla->ancho) / 4 - 1;

    for (int i = 0; i < pantalla->ancho; i++) {
        for (int j = 0; j < pantalla->alto; j++) {
            coor_t posicion = {i, j};
            circulo(pantalla, radio/2, posicion, 2);
            circulo(pantalla, radio, posicion, 6);
        }
        if (i%2 == 0 && i != 0)
            pantalla->display[pantalla->alto / 2][i] = 'o';
    }
}

void bloque_pokebola (pantalla_t* pantalla, pokemon_t* pokemon) {

    pokemon_t pkm_vacio = {"Vacio", 0, 0, 0};
    if (pokemon) pkm_vacio = *pokemon;

    pantalla_t bloque;
    bloque.alto = (pantalla->alto * 2 ) / 3 , bloque.ancho = bloque.alto * 2;
    inicializar_matriz(&bloque);

    poner_cuadrado(pantalla);

    int largo = minimo((int) strlen(pkm_vacio.nombre), pantalla->ancho - 2);
    coor_t coor = {1, posicion_media(pkm_vacio.nombre, pantalla->ancho)};
    texto_2_pantalla(pantalla, pkm_vacio.nombre, largo, coor);

    pokebola(&bloque);
    coor.x = 2;
    coor.y = (pantalla->ancho - bloque.ancho) / 2;
    bloque_2_pantalla(pantalla, bloque, coor);

    int cant_atributos = 3;
    int ancho_atributos = pantalla->ancho / cant_atributos;

    if (pokemon) {
        char* atributos[4] = {"Ataque", "Defensa", "Velocidad"};
        int valores[4] = {pkm_vacio.ataque, pkm_vacio.defensa, pkm_vacio.velocidad};

        for (int i = 0; i < cant_atributos; i++) {
            coor_t coor = {pantalla->alto - 4, posicion_media(atributos[i], ancho_atributos) + ancho_atributos * i};
            texto_2_pantalla(pantalla, atributos[i], (int) strlen(atributos[i]), coor);
        }

        for (int i = 0; i < cant_atributos; i++) {
            char numero[4];
            numero_2_texto(numero, valores[i], 2);

            coor_t coor = {pantalla->alto - 3, posicion_media(numero, ancho_atributos) + ancho_atributos * i};
            texto_2_pantalla(pantalla, numero, (int) strlen(numero), coor);
        }
    }
}


int mostrar_entrenador(entrenador_t entrenador, bool lider, int iteracion) {

    int ancho_pkm = ANCHO_POKEMON;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO, pantalla.alto = ALTO;
    bloque.ancho = ANCHO, bloque.alto = ALTO;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    bloque.ancho = ancho_pkm;
    mostrar_columna_entrandor(&bloque, entrenador, lider);

    coor_t desfase = {1, 2};
    bloque_2_pantalla(&pantalla, bloque, desfase);
    inicializar_matriz(&bloque);

    int separacion = desfase.y * 2 + ancho_pkm;
    poner_lado_vertical(&pantalla, separacion);

    pokemon_t* pokemon = (pokemon_t*)lista_elemento_en_posicion(entrenador.pokemones, (size_t) iteracion);

    bloque.ancho = pantalla.ancho - separacion - 4;
    bloque.alto = (pantalla.alto * 3) / 4;
    bloque_pokebola(&bloque, pokemon);
    coor_t coor = {(pantalla.alto - bloque.alto) / 2, separacion + 2};
    bloque_2_pantalla(&pantalla, bloque, coor);

    imprimir_pantalla(pantalla);
    return entrenador.cant_pokemones;
}

int mostrar_gimnasio(gimnasio_t gimnasio, int iteracion) {

    int ancho_pkm = ANCHO_POKEMON;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO, pantalla.alto = ALTO;
    bloque.ancho = ANCHO, bloque.alto = ALTO;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

    crear_titulo(&bloque, gimnasio.nombre, bloque.ancho);
    coor_t coor = {1, 0};

    bloque_2_pantalla(&pantalla, cambiar_pantalla(bloque, -1, 3), coor);
    inicializar_matriz(&bloque);

    int maximo = (pantalla.ancho/ancho_pkm) - 1 + iteracion;
    int cant_entrenadores = minimo(gimnasio.cant_entrenadores, maximo);
    int ancho_entrenadores = pantalla.ancho / cant_entrenadores;

    coor_t desfase = {3, (ancho_entrenadores - ancho_pkm) / 2};

    for (int i = iteracion; i < cant_entrenadores; i++) {
        size_t entrenador_posicion = (size_t) (gimnasio.cant_entrenadores - (i+1));
        entrenador_t entrenador = *(entrenador_t*)lista_elemento_en_posicion(gimnasio.entrenadores, entrenador_posicion);

        bloque.ancho = ancho_pkm;
        mostrar_columna_entrandor(&bloque, entrenador, i == 0);

        bloque_2_pantalla(&pantalla, bloque, desfase);
        desfase.y += ancho_entrenadores;
        inicializar_matriz(&bloque);
    }

    imprimir_pantalla(pantalla);
    return cant_entrenadores - iteracion;
}
