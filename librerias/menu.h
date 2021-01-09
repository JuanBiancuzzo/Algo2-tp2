#ifndef __MENU_H__
#define __MENU_H__

#include "herramientas.h"

#define INGRESAR_ARCHIVO 'E'
#define AGREGAR_GIMNASIO 'A'
#define COMENZAR_PARTIDA 'I'
#define SIMULAR_PARTIDA 'S'
#define MOSTRAR_ENTRENADOR 'E'
#define MOSTRAR_GIMNASIO 'G'
#define CAMBIAR_POKEMON 'C'
#define PROXIMA_BATALLA 'B'
#define PROXIMO_COMBATE 'N'
#define TOMAR_PRESTADO 'T'
#define CAMBIAR_POKEMONES 'C'
#define PROXIMO_GIMNASIO 'N'
#define REINTENTAR 'R'
#define FINALIZAR 'F'

#define AFIRMAR 'S'
#define NEGAR 'N'
#define SIGUIENTE 'S'
#define ANTERIOR 'A'

#define MAX_INSTRUC 10
#define ANCHO 100
#define ALTO 30

typedef void (*funcion_menu) (char[]);

/*
 * Teniendo las opciones de ingresar el archivo del personaje
 * principal, ingresar el archivo de un gimnasio, comenzar la
 * partida o simular la partida
 *
 * Devuelve los caracteres de las opciones que se muestran en
 * el menu
 */
void menu_inicio(char instrucciones[]);

/*
 * Teniendo las opciones de mostrar el entrenador, mostrar el
 * gimnasio, cambiar los pokemones y comenzar la batalla
 *
 * Devuelve los caracteres de las opciones que se muestran en
 * el menu
 */
void menu_gimnasio(char instrucciones[]);

/*
 * La unica "opcion" es ir a la siguiente batalla
 *
 * Devuelve los caracteres de las opciones que se muestran en
 * el menu
 */
void menu_batalla(char instrucciones[]);

/*
 * Teniendo las opciones de tomar prestado el pokemon del
 * lider, cambiar el orden de los pokemones e ir al proximo
 * gimnasio
 *
 * Devuelve los caracteres de las opciones que se muestran en
 * el menu
 */
void menu_victoria(char instrucciones[]);

/*
 * Teniendo las opciones de cambiar el orden de los pokemones
 * reintentar el gimnasio y finalizar la partida
 *
 * Devuelve los caracteres de las opciones que se muestran en
 * el menu
 */
void menu_derrota(char instrucciones[]);

/*
 * Mostrarle al usuario que tiene la opcion de afirmar o de
 * negar lo que se le pregunta
 */
void menu_confirmacion(char instrucciones[]);

/*
 * Deja al usuario navegar por los menus
 */
void menu_avanzar_retroceder(char instrucciones[]);

/*
 * Muestra dos pokemones, en el caso de que no este alguno de ellos
 * se mostrara el espacio disponible para el pokemon a elegir
 */
void mostrar_intercambiar_pokemones(pokemon_t* pkm1, pokemon_t* pkm2);

/*
 * Muestra todos los pokemones que hay en la lista, con los primeros
 * 6 separados, estos seria los que usarian para luchar
 */
int mostrar_pokemones(lista_t pokemones);

/*
 * Muestra al entrenador con todos sus pokemones, y con la variable
 * iteracion se puede ir viendo en mas detalle los pokemones del
 * entrenador
 */
int mostrar_entrenador(entrenador_t entrenador, bool lider, int iteracion);

/*
 * Muestra la estructura del gimnasio y con la variable iteracion
 * se puede ir viendo mas entrenadores que no entren en la pantalla
 */
int mostrar_gimnasio(gimnasio_t gimnasio, int iteracion);

#endif /* __HERRAMIENTAS_H__ */
