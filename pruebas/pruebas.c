#include <stdio.h>
#include <stdlib.h>

#include "pa2mm.h"
#include "../heap.h"

#define ERROR -1
#define EXITO 0

int comparador_prueba (void* elemento_uno, void* elemento_dos) {
    return *(int*) elemento_uno > *(int*) elemento_dos ? 1 : *(int*) elemento_uno < *(int*) elemento_dos ? -1 : 0;
}

void destructor_prueba (void* arbol) {
    arbol = arbol;
}

void probar_heap_crear_valores_invalidos () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = NULL;

    pa2m_afirmar((heap = heap_crear(NULL, destructor)) == NULL,
                 "Detecta correctamente que el comparador es invalido");

    pa2m_afirmar((heap = heap_crear(comparador, NULL)) != NULL,
                 "Detecta correctamente que no es necesario el destructor\n");

    heap_destruir(heap);
}

void probar_heap_crear_inicializacion () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = NULL;

    pa2m_afirmar((heap = heap_crear(comparador, destructor)) != NULL,
                 "Mensaje de creacion correcta al crear el arbol");

    pa2m_afirmar(!heap->elementos,
                 "Los elementos se inicializan correctamente");

    pa2m_afirmar(heap->cant_elementos == 0,
                 "La cantidad de elementos se inicializan correctamente");

    pa2m_afirmar(heap->comparador == comparador,
                 "El comparador se inicializan correctamente");

    pa2m_afirmar(heap->destructor == destructor,
                 "El destructor se inicializan correctamente");

    heap_destruir(heap);
}

void probar_heap_crear () {
    probar_heap_crear_valores_invalidos();
    probar_heap_crear_inicializacion();
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
