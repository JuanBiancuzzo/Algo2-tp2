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
void inicializar_matriz(char matriz[MAX_ANCHO][MAX_ALTO], int ancho, int alto){
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            matriz[i][j] = ' ';
}

/*
 * Dado una pantalla, los imprime con un borde
 * blanco
 */
void imprimir_pantalla (char pantalla[MAX_ANCHO][MAX_ALTO], int ancho, int alto) {
    linea_blanca(ancho + 4);
    printf("\n");
    for (int i = 0; i < alto; i++) {
        linea_blanca(2);
        printf("%s", pantalla[i]);
        linea_blanca(2);
        printf("\n");
    }
    linea_blanca(ancho + 4);
    printf("\n");
}

/*
 * Inserta una texto en la pantalla, en una pocicion
 * x e y
 */
void texto_2_pantalla (char pantalla[MAX_ANCHO][MAX_ALTO], char* texto, int largo, int x, int y) {
    for (int i = 0; i < largo; i++)
        pantalla[x][y+i] = *(texto + i);
}

/*
 * Inserta el bloque de textos en la pantalla en
 * las posiciones x e y
 */
void bloque_2_pantalla (char pantalla[MAX_ANCHO][MAX_ALTO],\
                        char bloque[MAX_ANCHO][MAX_ALTO], int alto_bloque, int ancho_bloque,\
                        int x, int y) {

    for (int i = 0; i < alto_bloque; i++)
        texto_2_pantalla(pantalla, bloque[i], ancho_bloque, x+i, y);
}

/*
 * Dado una serie de instrucciones, devuelve una
 * bloque con los elementos centrados en este
 */
void crear_instruccion (char bloque[MAX_ANCHO][MAX_ALTO], \
                        char letra, char* instruc[MAX_INSTRUC], int tope_instrucciones, \
                        int espacio)  {

    char caracter[4] = {'[', letra, ']'};
    texto_2_pantalla(bloque, caracter, 3, 0, posicion_media(caracter, espacio));

    for (int i = 0; i < tope_instrucciones; i++)
        texto_2_pantalla(bloque, instruc[i], (int) strlen(instruc[i]),  \
                         1 + i, posicion_media(instruc[i], espacio));
}

void menu_inicio(char* instrucciones) {

    int ancho = ANCHO, alto = 6, linea_inicial = 1;

    char pantalla[MAX_ANCHO][MAX_ALTO], bloque[MAX_ANCHO][MAX_ALTO];
    int cant_instrucciones = 4, cant_lineas = 3;
    int ancho_bloque = ancho / cant_instrucciones, alto_bloque = alto - 2;

    inicializar_matriz(pantalla, ancho, alto);
    inicializar_matriz(bloque, ancho_bloque, alto_bloque);

    char instruc[5] = {INGRESAR_ARCHIVO, AGREGAR_GIMNASIO, COMENZAR_PARTIDA, SIMULAR_PARTIDA};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Archivo", "del personaje" , "principal"}, \
                                            {"Agregar" , "archivo" , "del gimnasio"}, \
                                            {"Comenzar", "partida" , " "}, \
                                            {"Simular" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(bloque, instruc[i], items[i], cant_lineas, ancho_bloque);
        bloque_2_pantalla(pantalla, bloque, alto_bloque, ancho_bloque, linea_inicial, ancho_bloque * i);
        inicializar_matriz(bloque, ancho_bloque, alto_bloque);
    }

    imprimir_pantalla(pantalla, ancho, alto);
}

void menu_gimnasio(char instrucciones[]) {

    int ancho = ANCHO, alto = 6, linea_inicial = 1;

    char pantalla[MAX_ANCHO][MAX_ALTO], bloque[MAX_ANCHO][MAX_ALTO];
    int cant_instrucciones = 4, cant_lineas = 3;
    int ancho_bloque = ancho / cant_instrucciones, alto_bloque = alto - 2;

    inicializar_matriz(pantalla, ancho, alto);
    inicializar_matriz(bloque, ancho_bloque, alto_bloque);

    char instruc[5] = {MOSTRAR_ENTRENADOR, MOSTRAR_GIMNASIO, CAMBIAR_POKEMON, PROXIMA_BATALLA};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Mostrar el", "entrenador" , " "}, \
                                            {"Mostrar el" , "gimnasio" , " "}, \
                                            {"Cambiar el", "orden de tus" , "pokemones"}, \
                                            {"Proxima" , "batalla" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(bloque, instruc[i], items[i], cant_lineas, ancho_bloque);
        bloque_2_pantalla(pantalla, bloque, alto_bloque, ancho_bloque, linea_inicial, ancho_bloque * i);
        inicializar_matriz(bloque, ancho_bloque, alto_bloque);
    }

    imprimir_pantalla(pantalla, ancho, alto);
}

void menu_batalla(char instrucciones[]) {

    int ancho = ANCHO, alto = 5, linea_inicial = 1;

    char pantalla[MAX_ANCHO][MAX_ALTO], bloque[MAX_ANCHO][MAX_ALTO];
    int cant_instrucciones = 1, cant_lineas = 2;
    int ancho_bloque = ancho / cant_instrucciones, alto_bloque = alto - 2;

    inicializar_matriz(pantalla, ancho, alto);
    inicializar_matriz(bloque, ancho_bloque, alto_bloque);

    char instruc[2] = {PROXIMO_COMBATE};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Proximo", "combate"}};

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(bloque, instruc[i], items[i], cant_lineas, ancho_bloque);
        bloque_2_pantalla(pantalla, bloque, alto_bloque, ancho_bloque, linea_inicial, ancho_bloque * i);
        inicializar_matriz(bloque, ancho_bloque, alto_bloque);
    }

    imprimir_pantalla(pantalla, ancho, alto);
}

void menu_victoria(char instrucciones[]) {

    int ancho = ANCHO, alto = 6, linea_inicial = 1;

    char pantalla[MAX_ANCHO][MAX_ALTO], bloque[MAX_ANCHO][MAX_ALTO];
    int cant_instrucciones = 3, cant_lineas = 3;
    int ancho_bloque = ancho / cant_instrucciones, alto_bloque = alto - 2;

    inicializar_matriz(pantalla, ancho, alto);
    inicializar_matriz(bloque, ancho_bloque, alto_bloque);

    char instruc[4] = {TOMAR_PRESTADO, CAMBIAR_POKEMONES, PROXIMO_GIMNASIO};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Tomar prestado", "un pokemon"   , "del lider"}, \
                                            {"Cambiar el"    , "orden de tus" , "pokemones"}, \
                                            {"Proximo"       , "gimnasio"     , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(bloque, instruc[i], items[i], cant_lineas, ancho_bloque);
        bloque_2_pantalla(pantalla, bloque, alto_bloque, ancho_bloque, linea_inicial, ancho_bloque * i);
        inicializar_matriz(bloque, ancho_bloque, alto_bloque);
    }

    imprimir_pantalla(pantalla, ancho, alto);
}

void menu_derrota(char instrucciones[]) {

    int ancho = ANCHO, alto = 6, linea_inicial = 1;

    char pantalla[MAX_ANCHO][MAX_ALTO], bloque[MAX_ANCHO][MAX_ALTO];
    int cant_instrucciones = 3, cant_lineas = 3;
    int ancho_bloque = ancho / cant_instrucciones, alto_bloque = alto - 2;

    inicializar_matriz(pantalla, ancho, alto);
    inicializar_matriz(bloque, ancho_bloque, alto_bloque);

    char instruc[4] = {CAMBIAR_POKEMONES, REINTENTAR, FINALIZAR};
    strcpy(instrucciones, instruc);

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{"Cambiar el", "orden de tus" , "pokemones"}, \
                                            {"Reintentar", "gimnasio" , " "}, \
                                            {"Finalizar" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++) {
        crear_instruccion(bloque, instruc[i], items[i], cant_lineas, ancho_bloque);
        bloque_2_pantalla(pantalla, bloque, alto_bloque, ancho_bloque, linea_inicial, ancho_bloque * i);
        inicializar_matriz(bloque, ancho_bloque, alto_bloque);
    }

    imprimir_pantalla(pantalla, ancho, alto);
}
