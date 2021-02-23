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

#define GIMNASIOS_MINIMO 1
#define POKEMONES_MINIMO 1
#define GANO 1
#define PIERDE -1
#define TERMINA 0
#define JUGANDO 2

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
    if (!mapa)
        return false;
    return heap_elementos(mapa->gimnasios) >= GIMNASIOS_MINIMO;
}

/*
 * Para que el personaje principal este listo para el juego tiene que
 * tener por lo un pokemon
 */
bool principal_preparado(entrenador_t* principal) {
    if (!principal)
        return false;
    return lista_elementos(principal->pokemones) >= POKEMONES_MINIMO;
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
void preguntar_archivo(char* carpeta_destino, char nombre_archivo[]) {

    char ruta_archivo[MAX_NOMBRE];
    FILE* archivo;

    CLEAR;
    pantalla_titulo(NULL);
    printf("Por favor ingrese el nombre del archivo que tiene que estar en %s: ", carpeta_destino);
    scanf(" %80[^\n]", nombre_archivo);

    strcpy(ruta_archivo, carpeta_destino);
    strcat(ruta_archivo, nombre_archivo);

    archivo = fopen(ruta_archivo, "r");

    while(!archivo) {
        CLEAR;
        pantalla_titulo(NULL);
        printf("El archivo que indicaste no es valido. Revisar si ingreso correctamente el nombre \n");
        printf("del archivo o no coloco el archivo en la carpeta %s: ", carpeta_destino);
        scanf(" %80[^\n]", nombre_archivo);
 
        strcpy(ruta_archivo, carpeta_destino);
        strcat(ruta_archivo, nombre_archivo);

        archivo = fopen(ruta_archivo, "r");
    }

    fclose(archivo);
}

entrenador_t* cargar_entrenador(char* ruta) {
    char ruta_archivo[MAX_NOMBRE] = {CARPETA_PRINCIPAL};
    strcat(ruta_archivo, ruta);

    return archivo_2_personaje_principal(ruta_archivo);
}

gimnasio_t* cargar_gimnasio(char* ruta) {
    char ruta_archivo[MAX_NOMBRE] = {CARPETA_GIMNASIO};
    strcat(ruta_archivo, ruta);

    return archivo_2_gimnasio(ruta_archivo);
}

/*
 * Le pregunta al usuario que personaje principal quiere, y ese
 * archivo dado por el usuario se cargara un personaje principal
 *
 * En caso de que no quiera ese personaje principal, no se guardara
 */
void inicializar_personaje(entrenador_t** principal) {

    char nombre_archivo[MAX_NOMBRE], respuesta;

    preguntar_archivo(CARPETA_PRINCIPAL, nombre_archivo);
    entrenador_t* entrenador = cargar_entrenador(nombre_archivo);

    while (!entrenador) {
        CLEAR;
        mostrar_informacion("El personaje principal que ingresaste no es valido");        
        SLEEP;

        preguntar_archivo(CARPETA_PRINCIPAL, nombre_archivo);
        entrenador = cargar_entrenador(nombre_archivo);
    }

    CLEAR;
    respuesta = mostrar_menu(mostrar_principal, entrenador, menu_confirmacion, "Este es el personaje que queres usar?");
    CLEAR;
    if (responder_caracter(AFIRMAR, respuesta)) {
        destruir_entrenador(*principal);
        *principal = entrenador;
        mostrar_informacion("Se guardo correctamente el personaje principal");
    } else {
        destruir_entrenador(entrenador);
        mostrar_informacion("No se guardara ese personaje principal");
    }
    SLEEP;
}

/*
 * Le pregunta al usuario que gimnasio quiere agregar, y ese archivo
 * dado por el usuario se cargara un gimnasio
 *
 * Si el usuario acepta el gimnasio, se agregara al mapa, en caso de
 * que no quiera ese gimnasio, no se guardara en el mapa
 */
void inicializar_mapa(mapa_t* mapa) {

    char instrucciones[MAX_INSTRUC], nombre_archivo[MAX_NOMBRE], respuesta;
    gimnasio_t* gimnasio;

    preguntar_archivo(CARPETA_GIMNASIO, nombre_archivo);
    gimnasio = cargar_gimnasio(nombre_archivo);

    while (!gimnasio) {
        CLEAR;
        mostrar_informacion("El gimnasio que ingresaste no es valido");        
        SLEEP;

        preguntar_archivo(CARPETA_GIMNASIO, nombre_archivo);
        gimnasio = cargar_gimnasio(nombre_archivo);
    }

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

    CLEAR;
    if (responder_caracter(AFIRMAR, respuesta)) {
        gimnasio_2_mapa(mapa, gimnasio);
        mostrar_informacion("Se guardo correctamente el gimnasio");
    } else {
        destruir_gimnasio(gimnasio);
        mostrar_informacion("No se guardara ese gimnasio");
    }
    SLEEP;
}

/*
 * Desde el hub principal el usuario va a navegar las opciones dadas por
 * el menu_inicio y no se podra avanzar hasta que el mapa como el personaje
 * principal esten listos
 */
char hub_principal (mapa_t* mapa, entrenador_t** principal) {
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
             !juego_preparado(mapa, *principal)) {

            CLEAR;
            if (!principal_preparado(*principal))
                mostrar_informacion("El personaje principal todavia no esta listo");
            if (!mapa_preparado(mapa))
                mostrar_informacion("No se ingreso ningun gimnasio");
            SLEEP;
        }

    } while ((!responder_caracter(COMENZAR_PARTIDA, respuesta) && \
              !responder_caracter(SIMULAR_PARTIDA, respuesta)) ||  \
              !juego_preparado(mapa, *principal));

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
        printf("el primero es %s y el segundo %s\n", elegir_pokemon(principal->pokemones, 0)->nombre,  elegir_pokemon(principal->pokemones, 1)->nombre);
        printf("Elegi un numero de 1 al %i: ", (int) lista_elementos(principal->pokemones));
        scanf("%i", posicion+i);
        posicion[i]--;

        while (posicion[i] < 0 || posicion[i] >= (int) lista_elementos(principal->pokemones)) {
            CLEAR;
            mostrar_principal(principal);
            mostrar_intercambiar_pokemones(pkm[0], pkm[1], "Estos son los pokemones que elegiste");

            printf("Elegiste un numero no valido, contando de arriba a abajo, y de izquierda a derecha\n");
            printf("El primero es %s y el segundo %s\n", elegir_pokemon(principal->pokemones, 0)->nombre,  elegir_pokemon(principal->pokemones, 1)->nombre);
            printf("Elegi un numero de 1 al %i: ", (int) lista_elementos(principal->pokemones));
            scanf("%i", posicion+i);
            posicion[i]--;
        }

        pkm[i] = elegir_pokemon(principal->pokemones, posicion[i]);
    }
    CLEAR;
        mostrar_principal(principal);
        mostrar_intercambiar_pokemones(pkm[0], pkm[1], "Estos son los pokemones que elegiste");
    SLEEP;

    reordenar_pokemones(principal->pokemones, posicion[0], posicion[1]);

    CLEAR;
    respuesta = mostrar_menu(mostrar_principal, principal, menu_confirmacion, "Este es el cambio que quiere?");

    if (responder_caracter(NEGAR, respuesta))
        reordenar_pokemones(principal->pokemones, posicion[1], posicion[0]);

}

/*
 * Muestra al usuario al entrenador principal que es el 
 * personaje principal
 */
void mostrar_entrenador_principal(entrenador_t* principal) {

    char respuesta;
    bool quedarse = true;

    while(quedarse) {
        CLEAR;
        respuesta = mostrar_menu(mostrar_principal, principal, menu_confirmacion, "Queres salir?");
        if (responder_caracter(AFIRMAR, respuesta))
            quedarse = false;
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
        if ((int) lista_elementos(gimnasio->entrenadores) - cant_entrenadores <= 0) cant_entrenadores -= maximo;
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
            mostrar_entrenador_principal(principal);
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
int tomar_prestado_pokemon (entrenador_t* principal, entrenador_t* lider) {

    CLEAR;
    char instrucciones[MAX_INSTRUC], respuesta;
    int id_pokemon = 0;
    mostrar_entrenador(*lider, true, id_pokemon);
    printf("Elegir el pokemon, del numero 1 al %i, de arriba a abajo: ", (int) lista_elementos(lider->pokemones));
    scanf("%i", &id_pokemon);

    id_pokemon--;
    while (id_pokemon < 0 || id_pokemon >= (int) lista_elementos(lider->pokemones)) {
        id_pokemon = 0;
        CLEAR;
        mostrar_entrenador(*lider, true, id_pokemon);
        printf("Elegiste un numero invalido, tenes que ");
        printf("elegir el pokemon, del numero 1 al %i, de arriba a abajo: ", (int) lista_elementos(lider->pokemones));
        scanf("%i", &id_pokemon);

        id_pokemon--;
    }

    CLEAR;
    mostrar_entrenador(*lider, true, id_pokemon);
    menu_confirmacion(instrucciones, "Este es el pokemon que quieres tomar prestado?");
    scanf(" %c", &respuesta);

    while (!responder_opciones(instrucciones, respuesta)) {
        CLEAR;
        mostrar_entrenador(*lider, true, id_pokemon);
        menu_confirmacion(instrucciones, "Este es el pokemon que quieres tomar prestado?");
        printf("Tenes que elegir una de las opciones\n");
        scanf(" %c", &respuesta);
    }

    if (responder_caracter(NEGAR, respuesta))
        return ERROR;

    tomar_prestado(principal, lider, id_pokemon);
    return EXITO;
}

/*
 * En el hub victoria el usuario puede navegar las opciones dadas por
 * el menu_victoria solo cuando elija ir al proximo gimnasio
 */
char hub_victoria(entrenador_t* principal, entrenador_t* lider) {
    char respuesta;
    bool intercambio = false;

    do {
        CLEAR;
        respuesta = mostrar_menu(pantalla_victoria, NULL, menu_victoria, NULL);

        if (responder_caracter(TOMAR_PRESTADO, respuesta)) {
            if (!intercambio) {
                if (tomar_prestado_pokemon(principal, lider) == EXITO)
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

int pelear_gimnasio(gimnasio_t* gimnasio, entrenador_t** principal) {

    funcion_batalla estilo = estilo_batalla(gimnasio->id_funcion);
    int estado = JUGANDO;
    char respuesta;

    while (lista_elementos(gimnasio->entrenadores) > 0 && estado == JUGANDO) {
        entrenador_t* enemigo = entrenador_del_gimnasio(gimnasio);
        estado = batalla_pokemon(*principal, enemigo, estilo);
        CLEAR;

        if (estado == GANO) {
            hub_batalla(enemigo);
            if (enemigo->lider)
                hub_victoria(*principal, enemigo);
            sacar_entrenador(gimnasio);
            estado = JUGANDO;
        } else {
            respuesta = hub_derrota(*principal, enemigo);
            if (responder_caracter(FINALIZAR, respuesta))
                estado = TERMINA;
        }
    }

    return (estado == JUGANDO) ? GANO : estado;
}

/*
 * En encarga de hacer toda la partida, haciendo que el usario
 * pelee contra todos los gimnasios, mostrandole al usuario todas
 * las opciones que tiene devido cada situacion que se encuentre
 */
void comenzar_partida(mapa_t* mapa, entrenador_t* principal) {

    gimnasio_t* gimnasio = gimnasio_del_mapa(mapa);
    bool perdiste = false, termino = false;

    CLEAR;
    hub_gimnasio(principal, gimnasio);

    while ((heap_elementos(mapa->gimnasios) > 0 || perdiste) && !termino) {
        perdiste = false;

        int resultado = pelear_gimnasio(gimnasio, &principal);
        if (resultado == TERMINA) {
            termino = true;
        } else if (resultado == PIERDE){
            perdiste = true;
        } else {
            sacar_gimnasio(mapa);
            gimnasio = gimnasio_del_mapa(mapa);
        }
    }

    if (!perdiste && !termino)
        maestro_pokemon();
}

/*
 * En la simulacion de una batalla se le debe informar el resultado al
 * usuario y aumentar la cantidad de entrenadores que el usuario les
 * gano
 */
int simular_gimnasio(entrenador_t* principal, gimnasio_t* gimnasio) {

    funcion_batalla estilo = estilo_batalla(gimnasio->id_funcion);

    char mensaje[MAX_FRASE], nombre[MAX_NOMBRE];
    int resultado = GANO;
    while (lista_elementos(gimnasio->entrenadores) > 0 && resultado == GANO) {

        entrenador_t* enemigo = entrenador_del_gimnasio(gimnasio);
        resultado = batalla_pokemon(principal, enemigo, estilo);

        CLEAR;
        strcpy(nombre, enemigo->nombre);
        if (resultado == GANO) {
            pantalla_batalla(enemigo);
            strcpy(mensaje, "Ganaste contra ");
            sacar_entrenador(gimnasio);
        } else {
            pantalla_derrota(enemigo);
            strcpy(mensaje, "Perdiste contra ");
        }
        strcat(mensaje, nombre);
        mostrar_informacion(mensaje);
        SLEEP;
    }

    return resultado;
}

/*
 * Se encarga de simular toda la partida, sin que el usuario
 * tenga que ingresar ningun input en todo el torneo
 */
void simular_partida(mapa_t* mapa, entrenador_t* principal) {

    gimnasio_t* gimnasio = gimnasio_del_mapa(mapa);
    int estado = GANO;

    while (heap_elementos(mapa->gimnasios) > 0 && estado == GANO) {
        CLEAR;
        mostrar_gimnasio(*gimnasio, 0);
        SLEEP;

        CLEAR;
        estado = simular_gimnasio(principal, gimnasio);

        if (estado == GANO) {
            CLEAR;
            pantalla_victoria(NULL);
            mostrar_informacion("Yey, venciste este gimnasio");
            sacar_gimnasio(mapa);

            gimnasio = gimnasio_del_mapa(mapa);
            SLEEP;
        }
    }

    if (estado == GANO)
        maestro_pokemon();
}

/*
 * Con los valores dados al ejecutar el programa, se tiene un modo default donde carga
 * archivos predeterminados, y otro modo donde el primer argumento es el archivo del
 * personaje principal y el resto de los gimnasios
 */
void valores_iniciales(int argc, char* argv[], mapa_t* mapa, entrenador_t** principal) {
    if (argc < 2)
        return;

    gimnasio_t* gimnasio;

    if (strcmp(argv[1], "default") == 0) {
        *principal = cargar_entrenador("ash.txt");

        gimnasio = cargar_gimnasio("ciudad_celeste.txt");
        if (gimnasio)
            gimnasio_2_mapa(mapa, gimnasio);

        return;
    }

    *principal = cargar_entrenador(argv[1]);

    for (int i = 2; i < argc; i++) {
        gimnasio = cargar_gimnasio(argv[i]);
        if (gimnasio)
            gimnasio_2_mapa(mapa, gimnasio);
    }
}

int main(int argc, char* argv[]) {

    entrenador_t* principal = NULL;
    mapa_t* mapa = crear_mapa();

    valores_iniciales(argc, argv, mapa, &principal);
    if (argc > 1) {
        CLEAR;
        if (principal_preparado(principal))
            mostrar_informacion("Se ingreso correctamente el personaje principal");
        for (int i = 0; i < heap_elementos(mapa->gimnasios); i++)
            mostrar_informacion("Se ingreso correctamente un gimnasio");
        SLEEP;
    }

    char respuesta = hub_principal(mapa, &principal);
    if (responder_caracter(COMENZAR_PARTIDA, respuesta))
        comenzar_partida(mapa, principal);
    else if (responder_caracter(SIMULAR_PARTIDA, respuesta))
        simular_partida(mapa, principal);

    destruir_entrenador(principal);
    destruir_mapa(mapa);
    return 0;
}
