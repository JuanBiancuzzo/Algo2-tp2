#include <stdio.h>
#include <stdlib.h>

#include "pa2mm.h"
#include "../heap.h"

#define ERROR -1
#define EXITO 0

void probar_heap_crear () {

}

void probar_heap_insertar () {

}

void probar_heap_raiz () {

}

void probar_heap_destruir () {

}

int main () {

    pa2m_nuevo_grupo("Pruebas de heap");
    printf(" * Heap crear:\n");
    probar_heap_crear();
    printf("\n * Heap insertar:\n");
    probar_heap_insertar();
    printf("\n * Heap raiz:\n");
    probar_heap_raiz();
    printf("\n * Heap destruir:\n");
    probar_heap_destruir();


    pa2m_mostrar_reporte();

    return 0;
}
