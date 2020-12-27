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

int posicion_padre(int posicion) {
    return (posicion-1)/2;
}

void sift_up (heap_t* heap, int posicion) {

    int padre = posicion_padre(posicion);

    void* aux = heap->elementos[posicion];
    heap->elementos[posicion] = heap->elementos[padre];
    heap->elementos[padre] = aux;
}

void reordenar_heap(heap_t* heap, int posicion) {

    int padre = posicion_padre(posicion);

    if (padre < 0) return;

    int comparacion = heap->comparador(heap->elementos[posicion], heap->elementos[padre]);

    if (comparacion > 0) {
        sift_up(heap, posicion);
        reordenar_heap(heap, padre);
    }
}

int heap_insertar(heap_t* heap, void* elemento) {

    if (!heap) return ERROR;

    heap->elementos[heap->cant_elementos] = elemento;
    (heap->cant_elementos)++;

    reordenar_heap(heap, (heap->cant_elementos) - 1);
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
