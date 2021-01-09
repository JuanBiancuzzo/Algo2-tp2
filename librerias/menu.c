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
        int largo = ((int) strlen(pantalla.display[i]) < pantalla.ancho) ? (int) strlen(pantalla.display[i]) : pantalla.ancho;

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

void menu_confirmacion(char instrucciones[]) {

    int cant_instrucciones = 2, cant_lineas = 1, linea_inicial = 1;
    pantalla_t pantalla, bloque;
    pantalla.ancho = ANCHO;
    pantalla.alto = 4;
    bloque.ancho = pantalla.ancho / cant_instrucciones;
    bloque.alto = pantalla.alto - 2;

    inicializar_matriz(&pantalla);
    inicializar_matriz(&bloque);

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

/*
 * Si ancho o alto es menor a 0, se mantiene el ancho y alto,
 * respectivamente, de la pantalla
 */
pantalla_t cambiar_pantalla(pantalla_t pantalla, int ancho, int alto) {
    if (ancho >= 0) pantalla.ancho = ancho;
    if (alto >= 0) pantalla.alto = alto;
    return pantalla;
}

void bloque_estadistica(pantalla_t* bloque, char letra, int valor, coor_t desfase) {
    char frase[6] = {letra, ':', (char) (48 + (valor % 100) / 10), (char) (48 + (valor % 10))};
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

    }

    imprimir_pantalla(pantalla, ancho, alto);
}
