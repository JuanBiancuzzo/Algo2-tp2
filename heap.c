#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define EXITO 0
#define ERROR -1


heap_t* heap_crear(heap_comparador comparador, heap_liberar_elemento destructor) {

    if (!comparador)
        return NULL;

    heap_t* heap = calloc(1, sizeof(heap_t));

    if (!heap) return NULL;

    heap->comparador = comparador;
    heap->destructor = destructor;

    return heap;
}

int heap_insertar(heap_t* heap, void* elemento) {

    if (!heap) return ERROR;



    return EXITO;
}

void* heap_raiz(heap_t* heap) {

    if (!heap) return NULL;

    return NULL;
}

void heap_destruir(heap_t* heap) {

    if (!heap) return;

    if (heap->destructor)
        for (int i = 0; i < heap->cant_elementos; i++)
            heap->destructor(heap->elementos[i]);

    free(heap);
}
