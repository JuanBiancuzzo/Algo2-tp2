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

void probar_heap_insertar_valores_invalidos () {
    int elemento = 11;

    pa2m_afirmar(heap_insertar(NULL, &elemento) == ERROR,
                 "Detecta correctamente que el heap es invalido\n");

}

void probar_heap_insertar_un_elemento () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);
    int elemento = 11;

    pa2m_afirmar(heap_insertar(heap, &elemento) == EXITO,
                 "Mensaje de exito al insertar el elemento");

    pa2m_afirmar(elemento == *(int*)heap->elementos[0] ,
                 "Se inserta correctamente el elemenento en el heap\n");

    heap_destruir(heap);
}

void probar_heap_insertar_varios_elementos () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);
    int elemento_uno = 11, elemento_dos = 22, elemento_tres = 33;

    pa2m_afirmar(heap_insertar(heap, &elemento_uno) == EXITO,
                 "Mensaje de exito al insertar el primer elemento");

    pa2m_afirmar(heap_insertar(heap, &elemento_dos) == EXITO,
                 "Mensaje de exito al insertar el segunfo elemento");

    pa2m_afirmar(heap_insertar(heap, &elemento_tres) == EXITO,
                 "Mensaje de exito al insertar el tercer elemento");

    pa2m_afirmar(elemento_uno == *(int*)heap->elementos[0] ,
                 "\nSe inserta correctamente el primer elemenento en el heap");

    pa2m_afirmar(elemento_dos == *(int*)heap->elementos[1] ,
                 "Se inserta correctamente el segundo elemenento en el heap");

    pa2m_afirmar(elemento_tres == *(int*)heap->elementos[2] ,
                 "Se inserta correctamente el tercer elemenento en el heap");

    heap_destruir(heap);
}

void probar_heap_insertar () {
    probar_heap_insertar_valores_invalidos();
    probar_heap_insertar_un_elemento();
    probar_heap_insertar_varios_elementos();
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
