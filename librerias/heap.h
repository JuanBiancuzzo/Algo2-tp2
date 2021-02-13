#ifndef __HEAP_H__
#define __HEAP_H__

#define MAX_ELEMENTOS 50

/*
 * Compara dos elementos del heap
 * Devolviendo 1 si el primero es mas chico, 0 si
 * los dos son iguales o -1 si el segundo es mas
 * chico
 */
typedef int (*heap_comparador)(void*, void*);

/*
 * Se encarga de destruir cualquier elemento que
 * este en el heap, en el caso de que no sea
 * necesario este puede ser NULL
 */
typedef void (*heap_liberar_elemento)(void*);

typedef struct heap {
    void* elementos[MAX_ELEMENTOS];
    size_t cantidad;
    heap_comparador comparador;
    heap_liberar_elemento destructor;
} heap_t;

/*
 * Crea un heap minimal reservando memoria para su
 * estructura, y en el caso de tener una funcion para
 * destruir lo que se reserve
 *
 * Devuelve un puntero a ese heap
 */
heap_t* heap_crear(heap_comparador comparador, heap_liberar_elemento destructor);

/*
 * Inserta un el elemento en el heap, al ser un heap
 * minimal si el elemento tiene una clave baja este
 * subira en el heap
 * Devuelve 0 si lo logra, -1 si no
 */
int heap_insertar(heap_t* heap, void* elemento);

/*
 * Devuelve el elemento que esta en la raiz del heap
 * y al mismo tiempo lo elimina del heap
 */
void* heap_raiz(heap_t* heap);

/*
 * Elimina al raiz y reordena el heap para que sea 
 * un heap valido
 */
void heap_eliminar_raiz(heap_t* heap);

/*
 * Devuelve la cantidad de elementos almacenados en el heap
 */
size_t heap_elementos(heap_t* heap);

/*
 * Destruye el heap liberando la memoria que se reservo
 */
void heap_destruir(heap_t* heap);

#endif /* __HEAP_H__ */
