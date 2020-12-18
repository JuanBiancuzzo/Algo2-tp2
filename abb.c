#include <stdlib.h>

#include "abb.h"

#define ERROR -1
#define EXITO 0

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor) {

    if (!comparador)
        return NULL;

    abb_t* arbol = calloc(1, sizeof(abb_t));

    if (!arbol)
        return NULL;

    arbol->comparador = comparador;
    arbol->destructor = destructor;

    return arbol;
}

int arbol_insertar(abb_t* arbol, void* elemento) {

    if (!arbol)
        return ERROR;

    if (!arbol->nodo_raiz) {
        nodo_abb_t* nodo = calloc(1, sizeof(nodo_abb_t));

        if (!nodo) return ERROR;

        nodo->elemento = elemento;
        arbol->nodo_raiz = nodo;
        return EXITO;
    }

    int comparacion = arbol->comparador(elemento, arbol_raiz(arbol));
    nodo_abb_t* nodo_actual = arbol->nodo_raiz;

    arbol->nodo_raiz = comparacion > 0 ? arbol->nodo_raiz->derecha : arbol->nodo_raiz->izquierda;

    int valor = arbol_insertar(arbol, elemento);

    if (valor == ERROR) return ERROR;

    if (comparacion > 0)
        nodo_actual->derecha = arbol->nodo_raiz;
    else nodo_actual->izquierda = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual;

    return EXITO;
}

/*
 * Devuelve el nodo que se encuentra totalmente a la derecha
 */
nodo_abb_t* predecesor_inorden(nodo_abb_t* nodo) {
    if (!nodo->derecha)
        return nodo;

    return predecesor_inorden(nodo->derecha);
}

/*
 * Devuelve la rama izquierda, con el hijo izquierdo del predecesor
 * en el extremo derecho de la rama
*/
nodo_abb_t* reemplazar_predecesor(nodo_abb_t* rama, nodo_abb_t* predecesor_hijo) {
    if (!rama->derecha)
        return predecesor_hijo;

    rama->derecha = reemplazar_predecesor(rama->derecha, predecesor_hijo);
    return rama;
}

/*
 * Dado un árbol, elimina la raiz. Si tiene un hijo lo reemplaza el hijo que tiene. Si tiene dos hijos,
 * el predecesor inorden lo reemplazará y tomará como hijo derecho al hijo derecho de la raiz
 */
void arbol_borrar_raiz(abb_t* arbol) {
    nodo_abb_t* nodo_borrar = arbol->nodo_raiz;

    if (nodo_borrar->derecha && nodo_borrar->izquierda) {
        arbol->nodo_raiz = predecesor_inorden(nodo_borrar->izquierda);
        arbol->nodo_raiz->izquierda = reemplazar_predecesor(nodo_borrar->izquierda, arbol->nodo_raiz->izquierda);
        arbol->nodo_raiz->derecha = nodo_borrar->derecha;
    } else {
        arbol->nodo_raiz = nodo_borrar->izquierda ? nodo_borrar->izquierda : nodo_borrar->derecha;
    }

    if (arbol->destructor) arbol->destructor(nodo_borrar->elemento);
    free(nodo_borrar);
}

int arbol_borrar(abb_t* arbol, void* elemento) {

    if (!arbol) return ERROR;

    if (arbol_vacio(arbol)) return ERROR;

    int comparacion = arbol->comparador(elemento, arbol_raiz(arbol));

    if (comparacion == 0) {
        arbol_borrar_raiz(arbol);
        return EXITO;
    }

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;
    arbol->nodo_raiz = comparacion > 0 ? arbol->nodo_raiz->derecha : arbol->nodo_raiz->izquierda;

    if (arbol_borrar(arbol, elemento) == ERROR) return ERROR;

    if (comparacion > 0)
        nodo_actual->derecha = arbol->nodo_raiz;
    else nodo_actual->izquierda = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual;

    return EXITO;
}

void* arbol_buscar(abb_t* arbol, void* elemento) {

    if(!arbol)
        return NULL;

    if (!arbol->nodo_raiz)
        return NULL;

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;
    int comparacion = arbol->comparador(elemento, arbol_raiz(arbol));

    if (comparacion == 0)
        return arbol_raiz(arbol);

    arbol->nodo_raiz = comparacion > 0 ? arbol->nodo_raiz->derecha : arbol->nodo_raiz->izquierda;

    void* valor = arbol_buscar(arbol, elemento);
    arbol->nodo_raiz = nodo_actual;

    return valor;
}

void* arbol_raiz(abb_t* arbol) {
    if (!arbol)
        return NULL;

    if (arbol_vacio(arbol))
        return NULL;

    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol) {
    if (!arbol)
        return true;

    if (!arbol->nodo_raiz)
        return true;

    return false;
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array) return 0;

    if (arbol_vacio(arbol)) return 0;

    if(tamanio_array == 0) return 0;

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual->izquierda;
    size_t rama_izq = arbol_recorrido_inorden(arbol, array, tamanio_array-1);

    arbol->nodo_raiz = nodo_actual;
    *(array+rama_izq) = arbol_raiz(arbol);

    if (tamanio_array - rama_izq < 1) {
        arbol->nodo_raiz = nodo_actual;
        return tamanio_array;
    }

    tamanio_array -= rama_izq;

    arbol->nodo_raiz = nodo_actual->derecha;
    size_t rama_der = arbol_recorrido_inorden(arbol, array+(rama_izq+1), tamanio_array-1);

    arbol->nodo_raiz = nodo_actual;
    return 1 + rama_izq + rama_der;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array) return 0;

    if (arbol_vacio(arbol)) return 0;

    if(tamanio_array == 0) return 0;

    *(array) = arbol_raiz(arbol);

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual->izquierda;
    size_t rama_izq = arbol_recorrido_preorden(arbol, array+1, tamanio_array-1);

    if (tamanio_array - rama_izq < 1) {
        arbol->nodo_raiz = nodo_actual;
        return tamanio_array;
    }
    tamanio_array -= rama_izq;

    arbol->nodo_raiz = nodo_actual->derecha;
    size_t rama_der = arbol_recorrido_preorden(arbol, array+(rama_izq+1), tamanio_array-1);

    arbol->nodo_raiz = nodo_actual;
    return 1 + rama_izq + rama_der;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array) {
    if (!arbol || !array) return 0;

    if (arbol_vacio(arbol)) return 0;

    if(tamanio_array == 0) return 0;

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual->izquierda;
    size_t rama_izq = arbol_recorrido_postorden(arbol, array, tamanio_array);

    arbol->nodo_raiz = nodo_actual->derecha;
    size_t rama_der = arbol_recorrido_postorden(arbol, array+rama_izq, tamanio_array-rama_izq);

    if (tamanio_array - (rama_izq + rama_der) < 1) {
        arbol->nodo_raiz = nodo_actual;
        return tamanio_array;
    }
    tamanio_array -= (rama_izq + rama_der);

    arbol->nodo_raiz = nodo_actual;
    *(array+(rama_izq+rama_der)) = arbol_raiz(arbol);

    return 1 + rama_izq + rama_der;
}

/*
 * Dado un arbol lo recorre de abajo hacia arriba destruyendo (si tiene destructor)
 * cada hoja y liberandola, hasta dejar el arbol vacio
 */
void arbol_destruir_hojas(abb_t* arbol) {
    if (!arbol)
        return;

    if (arbol_vacio(arbol))
        return;

    nodo_abb_t* nodo_actual = arbol->nodo_raiz;

    arbol->nodo_raiz = nodo_actual->izquierda;
    arbol_destruir_hojas(arbol);

    arbol->nodo_raiz = nodo_actual->derecha;
    arbol_destruir_hojas(arbol);

    arbol->nodo_raiz = nodo_actual;

    if (arbol->destructor) arbol->destructor(arbol_raiz(arbol));
    free(arbol->nodo_raiz);
}

void arbol_destruir(abb_t* arbol) {
    if (!arbol)
        return;

    if (!arbol_vacio(arbol))
        arbol_destruir_hojas(arbol);

    free(arbol);
}

/*
 * Itera todo el arbol, de la forma voy a la izquierda, después en el que estoy y
 * por ultimo a la derecha
 */
bool abb_iterador_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador) {
    if (!nodo || !funcion) return true;

    bool rama_der = true, rama_izq = true;

    rama_izq = abb_iterador_inorden(nodo->izquierda, funcion, extra, contador);

    if (rama_izq) {
        (*contador)++;

        if (funcion(nodo->elemento, extra)) return false;

        rama_der = abb_iterador_inorden(nodo->derecha, funcion, extra, contador);
    }

    return rama_izq && rama_der;
}

/*
 * Itera todo el arbol, de la forma primero en el que estoy, después voy a la
 * izquierda y por ultimo a la derecha
*/
bool abb_iterador_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador) {
    if (!nodo || !funcion) return true;

    bool rama_izq = true, rama_der = true;

    (*contador)++;
    if (funcion(nodo->elemento, extra)) return false;

    rama_izq = abb_iterador_preorden(nodo->izquierda, funcion, extra, contador);

    if (rama_izq)
        rama_der = abb_iterador_preorden(nodo->derecha, funcion, extra, contador);

    return rama_izq && rama_der;
}

/*
 * Itera todo el arbol, de la forma voy a la izquierda, después a la derecha y
 * por ultimo en el que estoy
 */
bool abb_iterador_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, size_t* contador) {
    if (!nodo || !funcion) return true;

    bool rama_izq = true, rama_der = true;

    rama_izq = abb_iterador_postorden(nodo->izquierda, funcion, extra, contador);

    if (rama_izq)
        rama_der = abb_iterador_postorden(nodo->derecha, funcion, extra, contador);

    if (rama_izq && rama_der)
        (*contador)++;

    if (rama_izq && rama_der)
        if (funcion(nodo->elemento, extra)) return false;

    return rama_izq && rama_der;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra) {

    if (!arbol || !funcion) return 0;

    size_t contador = 0;

    if (recorrido == ABB_RECORRER_INORDEN)
        abb_iterador_inorden(arbol->nodo_raiz, funcion, extra, &contador);

    else if (recorrido == ABB_RECORRER_PREORDEN)
        abb_iterador_preorden(arbol->nodo_raiz, funcion, extra, &contador);

    else if (recorrido == ABB_RECORRER_POSTORDEN)
        abb_iterador_postorden(arbol->nodo_raiz, funcion, extra, &contador);

    return contador;
}
