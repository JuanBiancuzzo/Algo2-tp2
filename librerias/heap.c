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

/*
 * Intercambia el padre con el hijo, haciendo que le
 * padre suba en el heap
 */
void sift_up (heap_t* heap, int posicion) {

    int padre = posicion_padre(posicion);

    void* aux = heap->elementos[posicion];
    heap->elementos[posicion] = heap->elementos[padre];
    heap->elementos[padre] = aux;
}

/*
 * Dado una posicion va reorganizando el heap para que
 * vaya subiendo la posicion siempre que al hacerlo cumpla
 * las condiciones de heap
 */
void heap_subir(heap_t* heap, int posicion) {

    int padre = posicion_padre(posicion);

    if (padre < 0) return;

    int comparacion = heap->comparador(heap->elementos[posicion], heap->elementos[padre]);

    if (comparacion < 0) {
        sift_up(heap, posicion);
        heap_subir(heap, padre);
    }
}

int heap_insertar(heap_t* heap, void* elemento) {

    if (!heap) return ERROR;

    heap->elementos[heap->cant_elementos] = elemento;
    (heap->cant_elementos)++;

    heap_subir(heap, (heap->cant_elementos) - 1);
    return EXITO;
}

/*
 * Intercambia el hijo con el padre, haciendo que le
 * padre baje en el heap
 */
void sift_down (heap_t* heap, int hijo) {

    int padre = posicion_padre(hijo);

    void* aux = heap->elementos[hijo];
    heap->elementos[hijo] = heap->elementos[padre];
    heap->elementos[padre] = aux;
}

/*
 * Dado una posicion va reorganizando el heap para que
 * vaya bajando la posicion siempre que al hacerlo cumpla
 * las condiciones de heap
 */
void heap_bajar(heap_t* heap, int padre) {

    int hijo_izq = (2*padre)+1;
    int hijo_der = (2*padre)+2;

    int hijo = hijo_izq, comparacion;

    if (hijo_izq > heap->cant_elementos) return;

    if (hijo_der <= heap->cant_elementos) {
        comparacion = heap->comparador(heap->elementos[hijo_izq], heap->elementos[hijo_der]);
        hijo = (comparacion > 0) ? hijo_der : hijo_izq;
    }

    comparacion = heap->comparador(heap->elementos[padre], heap->elementos[hijo]);

    if (comparacion < 0) {
        sift_down(heap, hijo);
        heap_bajar(heap, hijo);
    }
}

void* heap_raiz(heap_t* heap) {

    if (!heap) return NULL;

    return heap->elementos[0];
}

void heap_eliminar_raiz(heap_t* heap) {

    if (!heap) return;

    if (heap->destructor)
        heap->destructor(heap_raiz(heap));

    heap->elementos[0] = heap->elementos[heap->cant_elementos-1];
    (heap->cant_elementos)--;

    heap_bajar(heap, 0);

}

void heap_destruir(heap_t* heap) {

    if (!heap) return;

    if (heap->destructor)
        for (int i = 0; i < heap->cant_elementos; i++)
            heap->destructor(heap->elementos[i]);

    free(heap);
}
