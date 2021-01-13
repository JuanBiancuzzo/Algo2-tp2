#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <unistd.h>
#define CLEAR system("clear")
#define SLEEP sleep(2)
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

#define ENTRENADOR_GANA -1
#define PRINCIPAL_GANA 1

bool responder_caracter (char opcion, char respuesta) {
    return (opcion == respuesta || opcion+32 == respuesta);
}

bool responder_opciones (char opciones[], char respuesta) {
    for (size_t i = 0; i < strlen(opciones); i++)
        if (responder_caracter(opciones[i], respuesta))
            return true;
    return false;
}

bool mapa_preparado(mapa_t* mapa) {
    return mapa->cant_gimnasios > 0;
}

bool principal_preparado(personaje_t* principal) {
    return principal->cant_pokemones > 0;
}

bool juego_preparado(mapa_t* mapa, personaje_t* principal) {
    return (mapa_preparado(mapa) && principal_preparado(principal));
}

funcion_batalla estilo_batalla(int id_funcion) {
    switch(id_funcion) {
        case 1: return funcion_batalla_1;
        case 2: return funcion_batalla_2;
        case 3: return funcion_batalla_3;
        case 4: return funcion_batalla_4;
        case 5: return funcion_batalla_5;
        default: return funcion_batalla_1;
    }
}

char mostrar_menu (funcion_menu menu, void* menu_aux, mostrar_pantalla pantalla, void* pantalla_aux) {

    pantalla(pantalla_aux);
    char instrucciones[MAX_INSTRUC];
    menu(instrucciones, menu_aux);

    char respuesta;
    scanf(" %c", &respuesta);

    while (!responder_opciones(instrucciones, respuesta)) {
        CLEAR;
        pantalla(pantalla_aux);
        menu(instrucciones, menu_aux);
        printf("Tenes que elegir una de las opciones\n");
        scanf(" %c", &respuesta);
    }
    return respuesta;
}

void preguntar_archivo(char* carpeta_destino, int (*funcion_validacion) (char*, void*), void* auxilear) {

    char ruta_archivo[MAX_NOMBRE], nombre_archivo[MAX_NOMBRE];
    bool error = false;
    FILE* archivo;

    do {
        CLEAR;
        pantalla_titulo(NULL);

        if (!error) {
            printf("Por favor ingrese el nombre del archivo que tiene que estar en %s: ", carpeta_destino);
        } else {
            printf("El archivo que indicaste no es valido. Revisar si ingreso correctamente el nombre \n");
            printf("del archivo o no coloco el archivo en la carpeta %s: ", carpeta_destino);
        }

        scanf(" %80[^\n]", nombre_archivo);

        strcpy(ruta_archivo, carpeta_destino);
        strcat(ruta_archivo, nombre_archivo);

        archivo = fopen(ruta_archivo, "r");

        error = false;
        if (!archivo) error = true;

        if (!error && funcion_validacion(ruta_archivo, auxilear) == ERROR) error = true;

    } while (error);

    fclose(archivo);
}

void inicializar_personaje(personaje_t* principal) {

    char respuesta;

    do {
        int (*funcion_validacion) (char*, void*) = archivo_2_personaje_principal;
        preguntar_archivo(CARPETA_PRINCIPAL, funcion_validacion, principal);

        CLEAR;
        mostrar_principal(*principal);
        menu_confirmacion(instrucciones, "Este es el personaje que queres usar?");
        scanf(" %c", &respuesta);

        while (!responder_opciones(instrucciones, respuesta)) {
            CLEAR;
            mostrar_principal(*principal);
            menu_confirmacion(instrucciones, "Este es el personaje que queres usar?");
            printf("Tenes que elegir una de las opciones\n");
            scanf(" %c", &respuesta);
        }

    } while (responder_caracter(NEGAR, respuesta));

}

void inicializar_mapa(mapa_t* mapa) {

    gimnasio_t* gimnasio = crear_gimnasio();
    char instrucciones[MAX_INSTRUC];
    char respuesta;

    do {
        int (*funcion_validacion) (char*, void*) = archivo_2_gimnasio;
        preguntar_archivo(CARPETA_GIMNASIO, funcion_validacion, gimnasio);

        CLEAR;
        mostrar_gimnasio(*gimnasio, 0);
        menu_confirmacion(instrucciones, "Este es el gimnasio que vas a luchar contra?");
        scanf(" %c", &respuesta);

        while (!responder_opciones(instrucciones, respuesta)) {
            CLEAR;
            mostrar_gimnasio(*gimnasio, 0);
            menu_confirmacion(instrucciones, "Este es el gimnasio que vas a luchar contra?");
            printf("Tenes que elegir una de las opciones\n");
            scanf(" %c", &respuesta);
        }

    } while (responder_caracter(NEGAR, respuesta));

    gimnasio_2_mapa(mapa, gimnasio);
}

char menu_principal (mapa_t* mapa, personaje_t* principal) {
    char respuesta;

    do {
        CLEAR;
        respuesta = iniciar_juego();
        if (responder_caracter(INGRESAR_ARCHIVO, respuesta))
            inicializar_personaje(principal);
        if (responder_caracter(AGREGAR_GIMNASIO, respuesta))
            inicializar_mapa(mapa);

    } while ((!responder_caracter(COMENZAR_PARTIDA, respuesta) && \
              !responder_caracter(SIMULAR_PARTIDA, respuesta)) ||  \
              !juego_preparado(mapa, principal));

    return respuesta;
}
int main() {
    CLEAR;

    return 0;
}
