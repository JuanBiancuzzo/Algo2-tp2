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
        respuesta = mostrar_menu(menu_confirmacion, "Este es el personaje que queres usar?", mostrar_principal, principal);

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

char hub_principal (mapa_t* mapa, personaje_t* principal) {
    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(menu_inicio, NULL, pantalla_titulo, NULL);
        if (responder_caracter(INGRESAR_ARCHIVO, respuesta))
            inicializar_personaje(principal);
        if (responder_caracter(AGREGAR_GIMNASIO, respuesta))
            inicializar_mapa(mapa);
        if ((responder_caracter(COMENZAR_PARTIDA, respuesta) ||         \
             responder_caracter(SIMULAR_PARTIDA, respuesta)) &&         \
             !juego_preparado(mapa, principal)) {
            
            if (!principal_preparado(principal))
                mostrar_informacion("El personaje principal todavia no esta listo");
            if (!mapa_preparado(mapa))
                mostrar_informacion("No se ingreso ningun gimnasio");
            SLEEP;
        }

    } while ((!responder_caracter(COMENZAR_PARTIDA, respuesta) && \
              !responder_caracter(SIMULAR_PARTIDA, respuesta)) ||  \
              !juego_preparado(mapa, principal));

    return respuesta;
}

void cambiar_pokemones(personaje_t* principal) {

    pokemon_t* pkm[2] = {NULL, NULL};
    int posicion[2];
    char respuesta;

    for (int i = 0; i < 2; i++) {
        CLEAR;
        mostrar_principal(principal);
        mostrar_intercambiar_pokemones(pkm[0], pkm[1], "Estos son los pokemones que elegiste");

        printf("Contando de arriba a abajo, y de izquierda a derecha, ");
        printf("el primero es %s y el segundo %s\n", elegir_pokemon(principal, 0)->nombre,  elegir_pokemon(principal, 1)->nombre);
        printf("Elegi un numero de 1 al %i: ", principal->cant_pokemones);
        scanf("%i", posicion+i);
        posicion[i]--;

        while (posicion[i] < 0 || posicion[i] >= principal->cant_pokemones) {
            CLEAR;
            mostrar_principal(principal);
            mostrar_intercambiar_pokemones(pkm[0], pkm[1], "Estos son los pokemones que elegiste");

            printf("Elegiste un numero no valido, contando de arriba a abajo, y de izquierda a derecha\n");
            printf("El primero es %s y el segundo %s\n", elegir_pokemon(principal, 0)->nombre,  elegir_pokemon(principal, 1)->nombre);
            printf("Elegi un numero de 1 al %i: ", principal->cant_pokemones);
            scanf("%i", posicion+i);
            posicion[i]--;
        }

        pkm[i] = elegir_pokemon(principal, posicion[i]);
    }

    reordenar_pokemones(principal->pokemones, posicion[0], posicion[1]);

    CLEAR;
    respuesta = mostrar_menu(menu_confirmacion, "Este es el cambio que quiere?", mostrar_principal, principal);

    if (responder_caracter(NEGAR, respuesta))
        reordenar_pokemones(principal->pokemones, posicion[1], posicion[0]);

}

void mostrar_entrenador_principal(gimnasio_t* gimnasio) {

    entrenador_t* entrenador = pelear_entrenador(gimnasio, gimnasio->cant_entrenadores - 1);
    char instrucciones[MAX_INSTRUC], respuesta;
    bool quedarse = true;
    int contador = 0;

    while (quedarse) {
        CLEAR;
        if (contador >= entrenador->cant_pokemones) contador--;
        if (contador < 0) contador = 0;

        mostrar_entrenador(*entrenador, true, contador);
        menu_avanzar_retroceder(instrucciones, NULL);
        scanf(" %c", &respuesta);

        while (!responder_opciones(instrucciones, respuesta)) {
            CLEAR;
            mostrar_entrenador(*entrenador, true, contador);
            menu_avanzar_retroceder(instrucciones, NULL);
            printf("Tenes que elegir una de las opciones\n");
            scanf(" %c", &respuesta);
        }

        if (responder_caracter(VOLVER, respuesta))
            quedarse = false;
        contador += (responder_caracter(SIGUIENTE, respuesta)) ? 1 : - 1;
    }

}

void mostrar_gimnasio_actual (gimnasio_t* gimnasio) {

    char instrucciones[MAX_INSTRUC], respuesta;
    int cant_entrenadores = 0, maximo = 4;
    bool quedarse = true;

    while (quedarse) {
        CLEAR;
        if (gimnasio->cant_entrenadores - cant_entrenadores <= 0) cant_entrenadores -= maximo;
        if (cant_entrenadores < 0) cant_entrenadores = 0;

        mostrar_gimnasio(*gimnasio, cant_entrenadores);
        menu_avanzar_retroceder(instrucciones, NULL);
        scanf(" %c", &respuesta);

        while (!responder_opciones(instrucciones, respuesta)) {
            CLEAR;
            cant_entrenadores = mostrar_gimnasio(*gimnasio, cant_entrenadores);
            menu_avanzar_retroceder(instrucciones, NULL);
            printf("Tenes que elegir una de las opciones\n");
            scanf(" %c", &respuesta);
        }

        if (responder_caracter(VOLVER, respuesta)) {
            quedarse = false;
        } else if (responder_caracter(SIGUIENTE, respuesta)) {
            cant_entrenadores += maximo;
        } else {
            cant_entrenadores -= maximo;
        }
    }

}

char hub_gimnasio (personaje_t* principal, gimnasio_t* gimnasio) {

    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(menu_gimnasio, NULL, pantalla_titulo, NULL);
        if (responder_caracter(MOSTRAR_ENTRENADOR, respuesta))
            mostrar_entrenador_principal(gimnasio);
        else if (responder_caracter(MOSTRAR_GIMNASIO, respuesta))
            mostrar_gimnasio_actual(gimnasio);
        else if (responder_caracter(CAMBIAR_POKEMONES, respuesta))
            cambiar_pokemones(principal);

    } while (!responder_caracter(PROXIMA_BATALLA, respuesta));

    return respuesta;
}

char hub_batalla(entrenador_t* entrenador) {
    return mostrar_menu(menu_batalla, NULL, pantalla_batalla, entrenador);
}


char hub_derrota(personaje_t* principal, entrenador_t* entrenador) {
    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(menu_derrota, NULL, pantalla_derrota, entrenador);

        if (responder_caracter(CAMBIAR_POKEMONES, respuesta))
            cambiar_pokemones(principal);

    } while (!responder_caracter(REINTENTAR, respuesta) && \
             !responder_caracter(FINALIZAR, respuesta));

    return respuesta;
}

void tomar_prestado_pokemon (personaje_t* principal, gimnasio_t* gimnasio) {

    CLEAR;
    entrenador_t* entrenador = pelear_entrenador(gimnasio, gimnasio->cant_entrenadores - 1);
    char instrucciones[MAX_INSTRUC], respuesta;
    int id_pokemon = 0;
    mostrar_entrenador(*entrenador, true, id_pokemon);
    printf("Elegir el pokemon, del numero 1 al %i, de arriba a abajo: ", entrenador->cant_pokemones);
    scanf("%i", &id_pokemon);

    id_pokemon--;
    while (id_pokemon < 0 || id_pokemon >= entrenador->cant_pokemones) {
        id_pokemon = 0;
        CLEAR;
        mostrar_entrenador(*entrenador, true, id_pokemon);
        printf("Elegiste un numero invalido, tenes que ");
        printf("elegir el pokemon, del numero 1 al %i, de arriba a abajo: ", entrenador->cant_pokemones);
        scanf("%i", &id_pokemon);

        id_pokemon--;
    }

    CLEAR;
    mostrar_entrenador(*entrenador, true, id_pokemon);
    menu_confirmacion(instrucciones, "Este es el pokemon que quieres tomar prestado?");
    scanf(" %c", &respuesta);

    while (!responder_opciones(instrucciones, respuesta)) {
        CLEAR;
        mostrar_entrenador(*entrenador, true, id_pokemon);
        menu_confirmacion(instrucciones, "Este es el pokemon que quieres tomar prestado?");
        printf("Tenes que elegir una de las opciones\n");
        scanf(" %c", &respuesta);
    }

    if (responder_caracter(AFIRMAR, respuesta))
        tomar_prestado(principal, entrenador, id_pokemon);
}

char hub_victoria(personaje_t* principal, gimnasio_t* gimnasio) {
    char respuesta;
    bool intercambio = false;

    do {
        CLEAR;
        respuesta = mostrar_menu(menu_victoria, NULL, pantalla_victoria, NULL);

        if (responder_caracter(TOMAR_PRESTADO, respuesta)) {
            if (!intercambio) {
                tomar_prestado_pokemon(principal, gimnasio);
                intercambio = true;
            } else {
                mostrar_informacion("Ya intercambiaste un pokemon con el lider");
                SLEEP;
            }
        } else if (responder_caracter(CAMBIAR_POKEMONES, respuesta)) {
            cambiar_pokemones(principal);
        }

    } while (!responder_caracter(PROXIMO_GIMNASIO, respuesta));

    return respuesta;
}

void maestro_pokemon() {
    CLEAR;
    pantalla_maestro_pokemon(NULL);
}

void comenzar_partida(mapa_t* mapa, personaje_t* principal) {

    gimnasio_t* gimnasio = pelar_gimnasio(mapa);
    funcion_batalla estilo = estilo_batalla(gimnasio->id_funcion);
    bool perdiste = false, termino = false;
    int contador = 0;
    char respuesta;

    CLEAR;
    respuesta = hub_gimnasio(principal, gimnasio);

    while ((mapa->cant_gimnasios >= 0 || perdiste) && !termino) {
        perdiste = false;

        while (contador < gimnasio->cant_entrenadores && !perdiste) {
            CLEAR;
            entrenador_t* enemigo = pelear_entrenador(gimnasio, contador);
            int resultado_batalla = batalla_pokemon(principal, enemigo, estilo);

            if (resultado_batalla == PRINCIPAL_GANA) {
                hub_batalla(enemigo);
                contador++;
            } else {
                perdiste = true;
                respuesta = hub_derrota(principal, enemigo);
                if (responder_caracter(FINALIZAR, respuesta))
                    termino = true;
            }
        }

        if (!perdiste) {
            hub_victoria(principal, gimnasio);
            gimnasio = pelar_gimnasio(mapa);
            estilo = estilo_batalla(gimnasio->id_funcion);
            contador = 0;
        }
    }

    if (!perdiste) {
        maestro_pokemon();
        mostrar_informacion("Yey, venciste todos los gimnasios");
    }
}


    return 0;
}
