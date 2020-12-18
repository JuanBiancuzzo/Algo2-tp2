#include <stdlib.h>
#include <stdio.h>

#include "lista.h"

#define EXITO 0
#define ERROR -1

lista_t* lista_crear() {

    lista_t* lista = calloc(1, sizeof(lista_t));

    if (!lista)
        return NULL;

    return lista;
}

int lista_insertar(lista_t* lista, void* elemento) {

    if (!lista)
        return ERROR;

    nodo_t* nodo = calloc(1, sizeof(nodo_t));

    if (!nodo)
        return ERROR;

    nodo->elemento = elemento;

    if (lista_vacia(lista))
        lista->nodo_inicio = nodo;
    else
        lista->nodo_fin->siguiente = nodo;

    lista->nodo_fin = nodo;
    lista->cantidad++;

    return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion) {

    if (!lista)
        return ERROR;

    if (posicion == 0) {
        lista_apilar(lista, elemento);
    } else if (posicion >= lista->cantidad) {
        lista_encolar(lista, elemento);
    } else {

        nodo_t* nodo = malloc(sizeof(nodo_t));

        if (!nodo)
            return ERROR;

        nodo->elemento = elemento;

        nodo_t* nodo_aux = lista->nodo_inicio;
        size_t contador = 1;

        while (contador < posicion && contador < lista->cantidad) {
            nodo_aux = nodo_aux->siguiente;
            contador++;
        }

        nodo->siguiente = nodo_aux->siguiente;
        nodo_aux->siguiente = nodo;

        lista->cantidad++;
    }

    return EXITO;
}

int lista_borrar(lista_t* lista) {
    if (!lista)
        return ERROR;

    if (lista_vacia(lista))
        return ERROR;

    nodo_t* nodo = lista->nodo_inicio;
    size_t contador = 2;

    while (contador < lista->cantidad) {
        nodo = nodo->siguiente;
        contador++;
    }

    free(lista->nodo_fin);
    lista->cantidad--;

    if (lista_vacia(lista)) {
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
    } else {
        nodo->siguiente = NULL;
        lista->nodo_fin = nodo;
    }

    return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion) {

    if (!lista)
        return ERROR;

    if (lista_vacia(lista))
        return ERROR;

    if (posicion == 0) {
        lista_desapilar(lista);
    } else if (posicion >= lista->cantidad){
        lista_borrar(lista);
    } else {

        nodo_t* nodo = lista->nodo_inicio;
        size_t contador = 1;

        while (contador < posicion && contador + 1 < lista->cantidad) {
            nodo = nodo->siguiente;
            contador++;
        }

        nodo_t* nodo_aux;
        nodo_aux = nodo->siguiente;
        nodo->siguiente = nodo_aux->siguiente;

        free(nodo_aux);

        lista->cantidad--;

    }

    return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){

    if (!lista)
        return NULL;

    if (lista_vacia(lista))
        return NULL;

    if (posicion > lista->cantidad)
        return NULL;

    nodo_t* nodo = lista->nodo_inicio;
    size_t contador = 0;

    while (contador < posicion) {
        nodo = nodo->siguiente;
        contador++;
    }

    return nodo->elemento;
}

void* lista_ultimo(lista_t* lista){
    if (!lista)
        return NULL;
    if (lista_vacia(lista))
        return NULL;
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    if (!lista)
        return true;

    if (lista->cantidad == 0)
        return true;

    return false;
}

size_t lista_elementos(lista_t* lista) {
    if (!lista)
        return 0;
    return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento) {
    if (!lista)
        return ERROR;

    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (!nodo)
        return ERROR;

    nodo->elemento = elemento;
    nodo->siguiente = lista->nodo_inicio;

    lista->nodo_inicio = nodo;
    lista->cantidad++;

    if (lista->cantidad == 1)
        lista->nodo_fin = nodo;

    return EXITO;
}

int lista_desapilar(lista_t* lista) {
    if (!lista)
        return ERROR;

    if (lista_vacia(lista))
        return ERROR;

    nodo_t* nodo_aux = lista->nodo_inicio->siguiente;

    free(lista->nodo_inicio);
    lista->nodo_inicio = nodo_aux;

    lista->cantidad--;
    if (lista_vacia(lista))
        lista->nodo_fin = NULL;

    return EXITO;
}

void* lista_tope(lista_t* lista) {
    if (!lista)
        return NULL;
    if (lista_vacia(lista))
        return NULL;
    return lista->nodo_inicio->elemento;
}

int lista_encolar(lista_t* lista, void* elemento) {
    if (!lista)
        return ERROR;

    nodo_t* nodo = malloc(sizeof(nodo_t));

    if (!nodo)
        return ERROR;

    nodo->elemento = elemento;
    nodo->siguiente = NULL;

    if (lista_vacia(lista))
        lista->nodo_inicio = nodo;
    else
        lista->nodo_fin->siguiente = nodo;

    lista->nodo_fin = nodo;
    lista->cantidad++;

    return EXITO;
}

int lista_desencolar(lista_t* lista) {
    if (!lista)
        return ERROR;

    if (lista_vacia(lista))
        return ERROR;

    nodo_t* nodo_aux = lista->nodo_inicio->siguiente;

    free(lista->nodo_inicio);
    lista->nodo_inicio = nodo_aux;

    lista->cantidad--;
    if (lista_vacia(lista))
        lista->nodo_fin = NULL;

    return EXITO;
}

void* lista_primero(lista_t* lista) {
    if (!lista)
        return NULL;

    if (lista_vacia(lista))
        return NULL;

    return lista->nodo_inicio->elemento;
}

void lista_destruir(lista_t* lista) {
    if (!lista)
        return;

    while (!lista_vacia(lista))
        lista_desapilar(lista);

    free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista) {
    if (!lista)
        return NULL;

    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));

    if (!iterador)
        return NULL;

    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;

    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador) {
    if (!iterador)
        return false;

    return iterador->corriente;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador) {
    if (!iterador)
        return false;

    if (!lista_iterador_tiene_siguiente(iterador))
        return false;

    iterador->corriente = iterador->corriente->siguiente;
    return iterador->corriente;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador) {
    if (!iterador)
        return NULL;

    if (!iterador->corriente)
        return NULL;

    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador) {
    free(iterador);
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void* contexto) {
    if (!lista)
        return 0;

    if (!funcion)
        return 0;

    if (lista_vacia(lista))
        return 0;

    nodo_t* nodo = lista->nodo_inicio;
    size_t contador = 0;
    bool seguir = true;

    while (contador < lista->cantidad && seguir) {

        seguir = funcion(nodo->elemento, contexto);

        nodo = nodo->siguiente;
        contador++;
    }

    return contador;
}
