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

#define MAX_INSTRUC 5

typedef char* (*funcion_menu) (char[]);

char* menu_inicio(char instrucciones[]);

char* menu_gimnasio(char instrucciones[]);

char* menu_batalla(char instrucciones[]);

char* menu_victoria(char instrucciones[]);

char* menu_derrota(char instrucciones[]);

#endif /* __HERRAMIENTAS_H__ */
