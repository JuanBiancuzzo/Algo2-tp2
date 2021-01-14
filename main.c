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
#define GIMNASIOS_MINIMO 1
#define POKEMONES_MINIMO 1

/*
 * Dado una letrar, devuelve true si esta en mayuscula o minuscula
 * es igual a la opcion dada, sino devuelve false
 */
bool responder_caracter (char opcion, char respuesta) {
    return (opcion == respuesta || opcion+32 == respuesta);
}

/*
 * Dado la respuesta del usuario, devuelve true si esta en las
 * opciones dadas
 */
bool responder_opciones (char opciones[], char respuesta) {
    for (size_t i = 0; i < strlen(opciones); i++)
        if (responder_caracter(opciones[i], respuesta))
            return true;
    return false;
}

/*
 * Para que un mapa este listo para el juego tiene que tener por lo
 * menos un gimnasio
 */
bool mapa_preparado(mapa_t* mapa) {
    return mapa->cant_gimnasios >= GIMNASIOS_MINIMO;
}

/*
 * Para que el personaje principal este listo para el juego tiene que
 * tener por lo un pokemon
 */
bool principal_preparado(entrenador_t* principal) {
    return principal->cant_pokemones >= POKEMONES_MINIMO;
}

bool juego_preparado(mapa_t* mapa, entrenador_t* principal) {
    return (mapa_preparado(mapa) && principal_preparado(principal));
}

/*
 * Dado la id de la funcion devuelve el estilo de batalla que se
 * tendra para ese gimnasio, el rango va de 1 a 5
 */
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

/*
 * Dado una pantalla y un meno, devuelve la respuesta del usuario
 * que pertenezca a una de las opciones dadas por el menu
 */
char mostrar_menu (mostrar_pantalla pantalla, void* pantalla_aux, funcion_menu menu, void* menu_aux) {

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

/*
 * Se le pide al usuario que ingrese un archivo en la carpeta_destino,
 * si no esta o la funcion validacion dan error se le vuelve a preguntar
 * al usuario hasta tener un archivo valido
 */
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

        if (!error && funcion_validacion(ruta_archivo, auxilear) == ERROR) {
            fclose(archivo);
            error = true;
        }

    } while (error);

    fclose(archivo);
}

/*
 * Le pregunta al usuario que personaje principal quiere, y ese
 * archivo dado por el usuario se cargara un personaje principal
 *
 * En caso de que no quiera ese personaje principal, no se guardara
 */
void inicializar_personaje(entrenador_t* principal) {

    char respuesta;
    entrenador_t auxilear;

    int (*funcion_validacion) (char*, void*) = archivo_2_personaje_principal;
    preguntar_archivo(CARPETA_PRINCIPAL, funcion_validacion, &auxilear);

    CLEAR;
    respuesta = mostrar_menu(mostrar_principal, &auxilear, menu_confirmacion, "Este es el personaje que queres usar?");

    if (responder_caracter(AFIRMAR, respuesta)) {
        *principal = auxilear;
    } else {
        CLEAR;
        mostrar_informacion("No se guardara ese personaje principal");
        SLEEP;
    }
}

/*
 * Le pregunta al usuario que gimnasio quiere agregar, y ese archivo
 * dado por el usuario se cargara un gimnasio
 *
 * Si el usuario acepta el gimnasio, se agregara al mapa, en caso de
 * que no quiera ese gimnasio, no se guardara en el mapa
 */
void inicializar_mapa(mapa_t* mapa) {

    gimnasio_t* gimnasio = crear_gimnasio();
    char instrucciones[MAX_INSTRUC];
    char respuesta;

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

    if (responder_caracter(AFIRMAR, respuesta)) {
        gimnasio_2_mapa(mapa, gimnasio);
    } else {
        destruir_gimnasio(gimnasio);
        CLEAR;
        mostrar_informacion("No se guardara ese gimnasio");
        SLEEP;
    }
}

/*
 * Desde el hub principal el usuario va a navegar las opciones dadas por
 * el menu_inicio y no se podra avanzar hasta que el mapa como el personaje
 * principal esten listos
 */
char hub_principal (mapa_t* mapa, entrenador_t* principal) {
    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(pantalla_titulo, NULL, menu_inicio, NULL);
        if (responder_caracter(INGRESAR_ARCHIVO, respuesta))
            inicializar_personaje(principal);
        if (responder_caracter(AGREGAR_GIMNASIO, respuesta))
            inicializar_mapa(mapa);

        if ((responder_caracter(COMENZAR_PARTIDA, respuesta) ||         \
             responder_caracter(SIMULAR_PARTIDA, respuesta)) &&         \
             !juego_preparado(mapa, principal)) {

            CLEAR;
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

/*
 * El usuario es capaz de elegir dos pokemones del personaje
 * principal e intercambiarlo
 */
void cambiar_pokemones(entrenador_t* principal) {

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
    respuesta = mostrar_menu(mostrar_principal, principal, menu_confirmacion, "Este es el cambio que quiere?");

    if (responder_caracter(NEGAR, respuesta))
        reordenar_pokemones(principal->pokemones, posicion[1], posicion[0]);

}

/*
 * Muestra al usuario el lider del gimnasio dado
 */
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

/*
 * Muestra al usuario el gimnasio con todos sus entrenadores
 */
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

/*
 * Desde el hub gimnasio el usuario va a navegar las opciones dadas por el
 * menu_gimnasio, y podra avazar cuando eliga ir a la porxima batalla
 */
char hub_gimnasio (entrenador_t* principal, gimnasio_t* gimnasio) {

    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(pantalla_titulo, NULL, menu_gimnasio, NULL);
        if (responder_caracter(MOSTRAR_ENTRENADOR, respuesta))
            mostrar_entrenador_principal(gimnasio);
        else if (responder_caracter(MOSTRAR_GIMNASIO, respuesta))
            mostrar_gimnasio_actual(gimnasio);
        else if (responder_caracter(CAMBIAR_POKEMONES, respuesta))
            cambiar_pokemones(principal);

    } while (!responder_caracter(PROXIMA_BATALLA, respuesta));

    return respuesta;
}

/*
 * El hub batalla tiene la unica opcion de ir a la proxima batalla
 * por lo que el usuario tiene que elegir esa opcion
 */
char hub_batalla(entrenador_t* entrenador) {
    return mostrar_menu(pantalla_batalla, entrenador, menu_batalla, NULL);
}

/*
 * Desde el hub derrota el usuario va a navega las opciones dadas por
 * el menu_derrota y podra avanzar cuando decida reintentar el gimnasio
 * o finalizar la partida
 */
char hub_derrota(entrenador_t* principal, entrenador_t* entrenador) {
    char respuesta;

    do {
        CLEAR;
        respuesta = mostrar_menu(pantalla_derrota, entrenador, menu_derrota, NULL);

        if (responder_caracter(CAMBIAR_POKEMONES, respuesta))
            cambiar_pokemones(principal);

    } while (!responder_caracter(REINTENTAR, respuesta) && \
             !responder_caracter(FINALIZAR, respuesta));

    return respuesta;
}

/*
 * El usuario es capaz de tomar prestado un pokemon del lider del gimnasio
 * entonces tiene que elegir uno de los pokemones del lider para quedarselo
 */
void tomar_prestado_pokemon (entrenador_t* principal, gimnasio_t* gimnasio) {

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

/*
 * En el hub victoria el usuario puede navegar las opciones dadas por
 * el menu_victoria solo cuando elija ir al proximo gimnasio
 */
char hub_victoria(entrenador_t* principal, gimnasio_t* gimnasio) {
    char respuesta;
    bool intercambio = false;

    do {
        CLEAR;
        respuesta = mostrar_menu(pantalla_victoria, NULL, menu_victoria, NULL);

        if (responder_caracter(TOMAR_PRESTADO, respuesta)) {
            if (!intercambio) {
                tomar_prestado_pokemon(principal, gimnasio);
                intercambio = true;
            } else {
                CLEAR;
                mostrar_informacion("Ya intercambiaste un pokemon con el lider");
                SLEEP;
            }
        } else if (responder_caracter(CAMBIAR_POKEMONES, respuesta)) {
            cambiar_pokemones(principal);
        }

    } while (!responder_caracter(PROXIMO_GIMNASIO, respuesta));

    return respuesta;
}

/*
 * Muestra al usuario que le gano todos los gimnasios y se
 * convirtio en maestro pokemon
 */
void maestro_pokemon() {
    CLEAR;
    pantalla_maestro_pokemon(NULL);
    mostrar_informacion("Ahora sos maestro pokemon");
}

/*
 * En encarga de hacer toda la partida, haciendo que el usario
 * pelee contra todos los gimnasios, mostrandole al usuario todas
 * las opciones que tiene devido cada situacion que se encuentre
 */
void comenzar_partida(mapa_t* mapa, entrenador_t* principal) {

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

    if (!perdiste)
        maestro_pokemon();
}

/*
 * Si en la simulacion el usuario gana una pelear, se le debe informar
 * y cargar el siguiente gimnasio para pelar
 */
void simular_victoria(mapa_t* mapa, gimnasio_t* gimnasio, funcion_batalla estilo) {

    CLEAR;
    pantalla_victoria(NULL);
    mostrar_informacion("Yey, venciste este gimnasio");

    gimnasio = pelar_gimnasio(mapa);
    estilo = estilo_batalla(gimnasio->id_funcion);
    SLEEP;

}

/*
 * En la simulacion de una batalla se le debe informar el resultado al
 * usuario y aumentar la cantidad de entrenadores que el usuario les
 * gano
 */
bool simular_batalla(int resultado, entrenador_t* enemigo, int* entrenadores_vencidos) {

    char mensaje[MAX_FRASE];
    if (resultado == PRINCIPAL_GANA) {
        pantalla_batalla(enemigo);
        strcpy(mensaje, "Ganaste contra ");
        (*entrenadores_vencidos)++;
    } else {
        pantalla_derrota(enemigo);
        strcpy(mensaje, "Perdiste contra ");
    }
    strcat(mensaje, enemigo->nombre);
    mostrar_informacion(mensaje);
    SLEEP;

    return !(resultado == PRINCIPAL_GANA);
}

/*
 * Se encarga de simular toda la partida, sin que el usuario
 * tenga que ingresar ningun input en todo el torneo
 */
void simular_partida(mapa_t* mapa, entrenador_t* principal) {

    gimnasio_t* gimnasio = pelar_gimnasio(mapa);
    funcion_batalla estilo = estilo_batalla(gimnasio->id_funcion);
    bool perdiste = false;
    int entrenadores_vencidos = 0;

    while (mapa->cant_gimnasios >= 0 && !perdiste) {
        perdiste = false;

        CLEAR;
        mostrar_gimnasio(*gimnasio, 0);
        SLEEP;

        while (entrenadores_vencidos < gimnasio->cant_entrenadores && !perdiste) {
            CLEAR;
            entrenador_t* enemigo = pelear_entrenador(gimnasio, entrenadores_vencidos);
            int resultado_batalla = batalla_pokemon(principal, enemigo, estilo);

            perdiste = simular_batalla(resultado_batalla, enemigo, &entrenadores_vencidos);
        }

        if (!perdiste) {
            simular_victoria(mapa, gimnasio, estilo);
            entrenadores_vencidos = 0;
        }
    }

    if (!perdiste)
        maestro_pokemon();
}

/*
 * Con los valores dados al ejecutar el programa, se tiene un modo default donde carga
 * archivos predeterminados, y otro modo donde el primer argumento es el archivo del
 * personaje principal y el resto de los gimnasios
 */
void valores_iniciales(int argc, char* argv[], mapa_t* mapa, entrenador_t* principal) {
    if (argc < 2)
        return;

    char ruta_archivo[MAX_NOMBRE] = {CARPETA_PRINCIPAL};
    gimnasio_t* gimnasio;
    gimnasio_t auxilear;

    if (strcmp(argv[1], "default") == 0) {
        strcat(ruta_archivo, "ash.txt");
        archivo_2_personaje_principal(ruta_archivo, principal);

        strcpy(ruta_archivo, CARPETA_GIMNASIO);
        strcat(ruta_archivo, "ciudad_celeste.txt");
        if (archivo_2_gimnasio("gimnasios/ciudad_celeste.txt", &auxilear) == EXITO) {
            gimnasio = crear_gimnasio();
            *gimnasio = auxilear;
            gimnasio_2_mapa(mapa, gimnasio);
        }
        return;
    }

    strcpy(ruta_archivo, CARPETA_PRINCIPAL);
    strcat(ruta_archivo, argv[1]);
    archivo_2_personaje_principal(ruta_archivo, principal);

    for (int i = 2; i < argc; i++) {
        strcpy(ruta_archivo, CARPETA_GIMNASIO);
        strcat(ruta_archivo, argv[i]);
        if (archivo_2_gimnasio(ruta_archivo, &auxilear) == EXITO) {
            gimnasio = crear_gimnasio();
            *gimnasio = auxilear;
            gimnasio_2_mapa(mapa, gimnasio);
        }
    }
}

int main(int argc, char* argv[]) {

    entrenador_t* principal = crear_personaje_principal();
    mapa_t* mapa = crear_mapa();

    valores_iniciales(argc, argv, mapa, principal);
    if (argc > 1) {
        CLEAR;
        if (principal_preparado(principal))
            mostrar_informacion("Se ingreso correctamente el personaje principal");
        for (int i = 0; i < mapa->cant_gimnasios; i++)
            mostrar_informacion("Se ingreso correctamente un gimnasio");
        SLEEP;
    }

    char respuesta = hub_principal(mapa, principal);
    if (responder_caracter(COMENZAR_PARTIDA, respuesta))
        comenzar_partida(mapa, principal);
    else if (responder_caracter(SIMULAR_PARTIDA, respuesta))
        simular_partida(mapa, principal);

    destruir_personaje_principal(principal);
    destruir_mapa(mapa);
    return 0;
}
