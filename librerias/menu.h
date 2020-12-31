#ifndef __MENU_H__
#define __MENU_H__

#define ANCHO 80

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

#define MAX_INSTRUC 10

typedef void (*funcion_menu) (char[]);

void menu_inicio(char instrucciones[]);

void menu_gimnasio(char instrucciones[]);

void menu_batalla(char instrucciones[]);

void menu_victoria(char instrucciones[]);

void menu_derrota(char instrucciones[]);

#endif /* __HERRAMIENTAS_H__ */
