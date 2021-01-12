#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#define CLEAR system("clear")
#define SLEEP sleep(1)
#elif _WIN32
#include <windows.h>
#define CLEAR system("cls")
#define SLEEP Sleep(100)
#endif

#include "librerias/herramientas.h"
#include "librerias/batallas.h"
#include "librerias/menu.h"

#define CARPETA_GIMNASIO "gimnasios/"
#define CARPETA_PRINCIPAL "principal/"

bool responder_caracter (char opcion, char respuesta) {
    return (opcion == respuesta || opcion+32 == respuesta);
}

bool responder_opciones (char opciones[], char respuesta) {
    for (size_t i = 0; i < strlen(opciones); i++)
        if (responder_caracter(opciones[i], respuesta))
            return true;
    return false;
}

bool juego_preparado(mapa_t* mapa, personaje_t* principal) {
    return (mapa->gimnasios && principal->pokemones);
}
int main() {
    CLEAR;

    return 0;
}
