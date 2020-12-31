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
#define MAX_LINEAS 5
#define MAX_INSTRUC 5

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
void texto_2_pantalla (char pantalla[MAX_ANCHO][MAX_ALTO], char* texto, int x, int y) {
    for (int i = 0; (size_t)i < strlen(texto); i++)
        pantalla[x][y+i] = *(texto + i);
}

/*
 * Inserta unas instrucciones en la pantalla, las
 * coloca centradas en el espacio dado, y con un
 * desfase en x e y
 */
void instruccion_2_pantalla (char pantalla[MAX_ANCHO][MAX_ALTO], int espacio, \
                             int desfase_x, int desfase_y, char* item[MAX_LINEAS], int tope_lineas) {

    for (int i = 0; i < tope_lineas; i++) {
        int posicion = desfase_y + posicion_media(item[i], espacio);
        texto_2_pantalla(pantalla, item[i], desfase_x + i, posicion);
    }
}

char* menu_inicio(char* instrucciones) {

    char pantalla[MAX_ANCHO][MAX_ALTO];
    int ancho = ANCHO, alto = 6, linea_inicial = 1;
    int espacio_item = ancho / 4;
    int cant_instrucciones = 4, cant_lineas = 4;
    inicializar_matriz(pantalla, ancho, alto);

    char instruc[5] = {INGRESAR_ARCHIVO, AGREGAR_GIMNASIO, COMENZAR_PARTIDA, SIMULAR_PARTIDA};
    strcpy(instrucciones, instruc);

    char caracter[MAX_INSTRUC][4] = {{ '[', instruc[0], ']' }, \
                                     { '[', instruc[1], ']' }, \
                                     { '[', instruc[2], ']' }, \
                                     { '[', instruc[3], ']' } };

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{caracter[0], "Ingresar", "archivo" , "del principal"}, \
                                            {caracter[1], "Agregar" , "archivo" , "del gimnasio"}, \
                                            {caracter[2], "Comenzar", "partida" , " "}, \
                                            {caracter[3], "Simular" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++)
        instruccion_2_pantalla(pantalla, espacio_item, linea_inicial, espacio_item * i, items[i], cant_lineas);

    imprimir_pantalla(pantalla, ancho, alto);

    return instrucciones;
}

char* menu_gimnasio(char instrucciones[]) {
    char pantalla[MAX_ANCHO][MAX_ALTO];
    int ancho = ANCHO, alto = 6, linea_inicial = 1;
    int cant_instrucciones = 4, cant_lineas = 4;
    int espacio_item = ancho / cant_instrucciones;
    inicializar_matriz(pantalla, ancho, alto);

    char instruc[5] = {MOSTRAR_ENTRENADOR, MOSTRAR_GIMNASIO, CAMBIAR_POKEMON, PROXIMA_BATALLA};
    strcpy(instrucciones, instruc);

    char caracter[MAX_INSTRUC][4] = {{ '[', instruc[0], ']' }, \
                                     { '[', instruc[1], ']' }, \
                                     { '[', instruc[2], ']' }, \
                                     { '[', instruc[3], ']' } };

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{caracter[0], "Mostrar el", "entrenador" , " "}, \
                                            {caracter[1], "Mostrar el" , "gimnasio" , " "}, \
                                            {caracter[2], "Cambiar el", "orden de tus" , "pokemones"}, \
                                            {caracter[3], "Proxima" , "batalla" , " "} };

    for (int i = 0; i < cant_instrucciones; i++)
        instruccion_2_pantalla(pantalla, espacio_item, linea_inicial, espacio_item * i, items[i], cant_lineas);

    imprimir_pantalla(pantalla, ancho, alto);

    return instrucciones;
}

char* menu_batalla(char instrucciones[]) {
    char pantalla[MAX_ANCHO][MAX_ALTO];
    int ancho = ANCHO, alto = 5, linea_inicial = 1;
    int cant_instrucciones = 1, cant_lineas = 3;
    int espacio_item = ancho / cant_instrucciones;
    inicializar_matriz(pantalla, ancho, alto);

    char instruc[2] = {PROXIMO_COMBATE};
    strcpy(instrucciones, instruc);

    char caracter[MAX_INSTRUC][4] = {{ '[', instruc[0], ']' }};

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{caracter[0], "Proximo", "combate"}};

    for (int i = 0; i < cant_instrucciones; i++)
        instruccion_2_pantalla(pantalla, espacio_item, linea_inicial, espacio_item * i, items[i], cant_lineas);

    imprimir_pantalla(pantalla, ancho, alto);

    return instrucciones;
}

char* menu_victoria(char instrucciones[]) {
    char pantalla[MAX_ANCHO][MAX_ALTO];
    int ancho = ANCHO, alto = 6, linea_inicial = 1;
    int cant_instrucciones = 3, cant_lineas = 4;
    int espacio_item = ancho / cant_instrucciones;
    inicializar_matriz(pantalla, ancho, alto);

    char instruc[4] = {TOMAR_PRESTADO, CAMBIAR_POKEMONES, PROXIMO_GIMNASIO};
    strcpy(instrucciones, instruc);

    char caracter[MAX_INSTRUC][4] = {{ '[', instruc[0], ']' }, \
                                     { '[', instruc[1], ']' }, \
                                     { '[', instruc[2], ']' } };

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{caracter[0], "Tomar prestado", "un pokemon" , "del lider"}, \
                                            {caracter[1], "Cambiar el", "orden de tus" , "pokemones"}, \
                                            {caracter[2], "Proximo" , "gimnasio" , " "} };

    for (int i = 0; i < cant_instrucciones; i++)
        instruccion_2_pantalla(pantalla, espacio_item, linea_inicial, espacio_item * i, items[i], cant_lineas);

    imprimir_pantalla(pantalla, ancho, alto);

    return instrucciones;
}

char* menu_derrota(char instrucciones[]) {
    char pantalla[MAX_ANCHO][MAX_ALTO];
    int ancho = ANCHO, alto = 6, linea_inicial = 1;
    int cant_instrucciones = 3, cant_lineas = 4;
    int espacio_item = ancho / cant_instrucciones;
    inicializar_matriz(pantalla, ancho, alto);

    char instruc[4] = {CAMBIAR_POKEMONES, REINTENTAR, FINALIZAR};
    strcpy(instrucciones, instruc);

    char caracter[MAX_INSTRUC][4] = {{ '[', instruc[0], ']' }, \
                                     { '[', instruc[1], ']' }, \
                                     { '[', instruc[2], ']' } };

    char* items[MAX_INSTRUC][MAX_LINEAS] = {{caracter[0], "Cambiar el", "orden de tus" , "pokemones"}, \
                                            {caracter[1], "Reintentar", "gimnasio" , " "}, \
                                            {caracter[2], "Finalizar" , "partida" , " "} };

    for (int i = 0; i < cant_instrucciones; i++)
        instruccion_2_pantalla(pantalla, espacio_item, linea_inicial, espacio_item * i, items[i], cant_lineas);

    imprimir_pantalla(pantalla, ancho, alto);

    return instrucciones;
}
