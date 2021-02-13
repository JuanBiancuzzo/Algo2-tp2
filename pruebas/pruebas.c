#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pa2mm.h"
#include "../librerias/heap.h"
#include "../librerias/herramientas.h"

#define ERROR -1
#define EXITO 0
#define GANO_PRINCIPAL 1
#define GANO_ENEMIGO -1

int comparador_prueba (void* elemento_uno, void* elemento_dos) {
    return *(int*) elemento_uno > *(int*) elemento_dos ? 1 : *(int*) elemento_uno < *(int*) elemento_dos ? -1 : 0;
}

void destructor_prueba (void* heap) {
    heap = heap;
}

typedef struct estructura_contador {
    int key;
    int contador;
} contador_t;

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

    pa2m_afirmar(heap_elementos(heap) == 0,
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

    pa2m_afirmar(heap_insertar(heap, &elemento_tres) == EXITO,
                 "Mensaje de exito al insertar el primer elemento");

    pa2m_afirmar(heap_insertar(heap, &elemento_dos) == EXITO,
                 "Mensaje de exito al insertar el segundo elemento");

    pa2m_afirmar(heap_insertar(heap, &elemento_uno) == EXITO,
                 "Mensaje de exito al insertar el tercer elemento");

    pa2m_afirmar(elemento_uno == *(int*)heap->elementos[0] ,
                 "Se inserta correctamente el primer elemenento en el heap");

    pa2m_afirmar(elemento_tres == *(int*)heap->elementos[1] ,
                 "Se inserta correctamente el segundo elemenento en el heap");

    pa2m_afirmar(elemento_dos == *(int*)heap->elementos[2] ,
                 "Se inserta correctamente el tercer elemenento en el heap");

    heap_destruir(heap);
}

void probar_heap_insertar () {
    probar_heap_insertar_valores_invalidos();
    probar_heap_insertar_un_elemento();
    probar_heap_insertar_varios_elementos();
}

void probar_heap_raiz_valores_invalidos () {
    pa2m_afirmar(heap_raiz(NULL) == NULL,
                 "Detecta correctamente que el heap es invalido\n");
}

void probar_heap_raiz_con_un_elemento () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento = 11;
    heap_insertar(heap, &elemento);

    pa2m_afirmar(elemento == *(int*)heap_raiz(heap),
                 "Devuelve el valor correcto con un heap de un elemento\n");

    heap_destruir(heap);
}

void probar_heap_raiz_con_varios_elementos () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento_uno = 11, elemento_dos = 22, elemento_tres = 33;
    heap_insertar(heap, &elemento_dos);
    heap_insertar(heap, &elemento_tres);
    heap_insertar(heap, &elemento_uno);

    pa2m_afirmar(elemento_uno == *(int*)heap_raiz(heap),
                 "Devuelve el valor correcto con un heap de 3 elementos\n");

    heap_destruir(heap);
}

void probar_heap_raiz () {
    probar_heap_raiz_valores_invalidos();
    probar_heap_raiz_con_un_elemento();
    probar_heap_raiz_con_varios_elementos();
}

void probar_heap_eliminar_raiz_con_un_elemento () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento = 11;
    heap_insertar(heap, &elemento);
    heap_eliminar_raiz(heap);

    pa2m_afirmar(heap_elementos(heap) == 0,
                 "Vuelve el contador a 0 despues de eliminar el primer elemento del heap\n");

    heap_destruir(heap);
}

void probar_heap_eliminar_raiz_con_varios_elementos () {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento_uno = 11, elemento_dos = 22, elemento_tres = 33;
    heap_insertar(heap, &elemento_dos);
    heap_insertar(heap, &elemento_tres);
    heap_insertar(heap, &elemento_uno);

    heap_eliminar_raiz(heap);

    pa2m_afirmar(heap_elementos(heap) == 2,
                 "Vuelve el contador a 2 despues de eliminar el primer elemento con un heap de 3 elementos");

    pa2m_afirmar(elemento_dos == *(int*)heap->elementos[0],
                 "Actualiza correctamente la estructura despues de sacar la raiz\n");

    heap_destruir(heap);
}

void probar_heap_eliminar_raiz () {
    probar_heap_eliminar_raiz_con_un_elemento();
    probar_heap_eliminar_raiz_con_varios_elementos();
}

void probar_heap_elementos_valores_validos() {
    pa2m_afirmar(heap_elementos(NULL) == 0,
       "Detecta correctamente que el heap es invalido\n");
}

void probar_heap_elementos_heap_un_elemento() {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento = 11;
    heap_insertar(heap, &elemento);    

    pa2m_afirmar(heap_elementos(heap) == 1,
                 "Devuelve que solo tiene un elemento\n");

    heap_destruir(heap);
}

void probar_heap_elementos_heap_muchos_elementos() {
    heap_comparador comparador = comparador_prueba;
    heap_liberar_elemento destructor = destructor_prueba;
    heap_t* heap = heap_crear(comparador, destructor);

    int elemento_uno = 11;
    heap_insertar(heap, &elemento_uno);    
    int elemento_dos = 12;
    heap_insertar(heap, &elemento_dos); 
    int elemento_tres = 13;
    heap_insertar(heap, &elemento_tres); 

    pa2m_afirmar(heap_elementos(heap) == 3,
                 "Devuelve que tiene 3 elementos\n");

    heap_destruir(heap);
}

void probar_heap_elementos () {
    probar_heap_elementos_valores_validos();
    probar_heap_elementos_heap_un_elemento();
    probar_heap_elementos_heap_muchos_elementos();
}

int comparador_contador (void* elemento_uno, void* elemento_dos) {

    if (((contador_t*)elemento_uno)->key > ((contador_t*)elemento_dos)->key)
        return 1;
    else if (((contador_t*)elemento_uno)->key < ((contador_t*)elemento_dos)->key)
        return -1;
    return 0;
}

void destructor_contador (void* dato) {
    ((contador_t*)dato)->contador++;
}

void probar_heap_destruir_varios_elementos () {
    heap_comparador comparador = comparador_contador;
    heap_liberar_elemento destructor = destructor_contador;
    heap_t* heap = heap_crear(comparador, destructor);

    contador_t datos[3];
    datos[0].key = 11;
    datos[0].contador = 0;
    datos[1].key = 22;
    datos[1].contador = 0;
    datos[2].key = 33;
    datos[2].contador = 0;

    for (int i = 0; i < 3; i++)
        heap_insertar(heap, datos+i);

    heap_destruir(heap);

    pa2m_afirmar(datos[0].contador == 1,
                  "Pasa por el primer elemento una unica vez");

    pa2m_afirmar(datos[1].contador == 1,
                  "Pasa por el segundo elemento una unica vez");

    pa2m_afirmar(datos[2].contador == 1,
                  "Pasa por el tercer elemento una unica vez");

}

void probar_heap_destruir () {
    probar_heap_destruir_varios_elementos();
}

void probar_archivo_2_personaje_principal_valores_invalidos () {

    entrenador_t* personaje;

    pa2m_afirmar((personaje = archivo_2_personaje_principal(NULL)) == NULL,
                 "Detecta correctamente que la ruta del archivo es invalido");

    destruir_entrenador(personaje);

    pa2m_afirmar((personaje = archivo_2_personaje_principal("algo_random.txt")) == NULL,
                 "Detecta correctamente que la ruta del archivo no lleva a ningun lado\n");

    destruir_entrenador(personaje);
}

void probar_archivo_2_personaje_principal_formato_invalidos_sin_entrenado () {

    entrenador_t* personaje;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/per_principal_formato_invalido_sin_entrenador.txt";

    pa2m_afirmar((personaje = archivo_2_personaje_principal(ruta_archivo)) == NULL,
                 "Reconoce que no hay entrenador y manda un error\n");

    destruir_entrenador(personaje);
}

void probar_archivo_2_personaje_principal_formato_invalidos_sin_pokemones () {

    entrenador_t* personaje;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/per_principal_formato_invalido_sin_pokemones.txt";

    pa2m_afirmar((personaje = archivo_2_personaje_principal(ruta_archivo)) == NULL,
                 "Reconoce que no hay pokemones y manda un error\n");

    destruir_entrenador(personaje);
}

void probar_archivo_2_personaje_principal_formato_invalidos_corrupto () {

    entrenador_t* personaje;

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/per_principal_formato_invalido_corrupto.txt");

    pa2m_afirmar((personaje = archivo_2_personaje_principal(ruta_archivo)),
                 "Mensaje de exito en un archivo donde en el medio de los pokemones hay un error");

    pa2m_afirmar(lista_elementos(personaje->pokemones) == 2,
                 "La cantidad de pokemones es 2 al encontrar el tercer pokemon con un error\n");

    destruir_entrenador(personaje);
}

void probar_archivo_2_personaje_principal_formato_validos () {

    entrenador_t* personaje;
    int cant_pokemones = 11;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/per_principal_formato_valido.txt";

    pa2m_afirmar((personaje = archivo_2_personaje_principal(ruta_archivo)),
                 "Mensaje de exito al convertir el archivo a personaje");

    pa2m_afirmar(strcmp(personaje->nombre, "Ash") == 0,
                 "Reconoce correctamente el nombre del personaje principal");

    pa2m_afirmar(lista_elementos(personaje->pokemones) == (size_t) cant_pokemones,
                 "Reconoce la cantidad correcta de pokemones");

    pa2m_afirmar(personaje->pokemones,
                 "Crea una lista con los pokemones");

    pa2m_afirmar((int)lista_elementos(personaje->pokemones) == cant_pokemones,
                 "La lista tiene la cantidad correcta de pokemones\n");

    destruir_entrenador(personaje);
}

void probar_archivo_2_personaje_principal () {
    probar_archivo_2_personaje_principal_valores_invalidos();
    printf("  · Archivo invalido, sin el entrenador:\n");
    probar_archivo_2_personaje_principal_formato_invalidos_sin_entrenado();
    printf("  · Archivo invalido, sin pokemones:\n");
    probar_archivo_2_personaje_principal_formato_invalidos_sin_pokemones();
    printf("  · Archivo invalido, error en el tercer pokemon:\n");
    probar_archivo_2_personaje_principal_formato_invalidos_corrupto();
    printf("  · Archivo valido:\n");
    probar_archivo_2_personaje_principal_formato_validos();
}

void probar_archivo_2_gimnasio_valores_invalidos () {

    gimnasio_t* gimnasio;

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(NULL)) == NULL,
                 "Detecta correctamente que la ruta del archivo es invalido");

    destruir_gimnasio(gimnasio);

    pa2m_afirmar((gimnasio = archivo_2_gimnasio("algo_random.txt")) == NULL,
                 "Detecta correctamente que la ruta del archivo no lleva a ningun lado\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_sin_gimnasio () {

    gimnasio_t* gimnasio;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_invalido_sin_gimnasio.txt";

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(ruta_archivo)) == NULL,
                 "Reconoce que no hay gimnasio y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_sin_lider () {

    gimnasio_t* gimnasio;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_invalido_sin_lider.txt";

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(ruta_archivo)) == NULL,
                 "Reconoce que el gimnasio no tiene lider y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_lider_sin_pokemon () {

    gimnasio_t* gimnasio;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_invalido_lider_sin_pokemon.txt";

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(ruta_archivo)) == NULL,
                 "Reconoce que el lider no tiene pokemones, por lo que no tiene un lider valido y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_entrenador_sin_pokemon () {

    gimnasio_t* gimnasio;
    int cant_entrenadores = 2;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_invalido_entrenador_sin_pokemon.txt";

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(ruta_archivo)),
                 "Aunque haya un entrenador invalido, lo ignora y sigue leyendo, y no manda error");

    pa2m_afirmar(lista_elementos(gimnasio->entrenadores) == (size_t)cant_entrenadores,
                 "Devuelve la cantidad correcta de entrenadores\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_validos () {

    gimnasio_t* gimnasio;
    int cant_entrenadores = 3;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_valido.txt";

    pa2m_afirmar((gimnasio = archivo_2_gimnasio(ruta_archivo)),
                 "Mensaje de exito al convertir el archivo a un gimnasio");

    pa2m_afirmar(strcmp(gimnasio->nombre, "Gimnasio de Ciudad Celeste") == 0,
                 "Reconoce correctamente el nombre del gimnasio");

    pa2m_afirmar(lista_elementos(gimnasio->entrenadores) == (size_t) cant_entrenadores,
                 "Reconoce la cantidad correcta de entrenadores");

    pa2m_afirmar(gimnasio->entrenadores,
                 "Crea una pila con los entrenadores");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio () {
    probar_archivo_2_gimnasio_valores_invalidos();
    printf("  · Archivo invalido, sin gimnasio:\n");
    probar_archivo_2_gimnasio_formato_invalidos_sin_gimnasio();
    printf("  · Archivo invalido, sin lider:\n");
    probar_archivo_2_gimnasio_formato_invalidos_sin_lider();
    printf("  · Archivo invalido, lider sin pokemones:\n");
    probar_archivo_2_gimnasio_formato_invalidos_lider_sin_pokemon();
    printf("  · Archivo invalido, entrenador sin pokemones:\n");
    probar_archivo_2_gimnasio_formato_invalidos_entrenador_sin_pokemon();
    printf("  · Archivo valido:\n");
    probar_archivo_2_gimnasio_formato_validos();
}

void probar_gimnasio_2_mapa_valores_invalidos () {

    mapa_t* mapa = crear_mapa();
    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_valido.txt";
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(gimnasio_2_mapa(NULL, gimnasio) == ERROR,
                 "Detecta correctamente que el mapa es invalido");

    pa2m_afirmar(gimnasio_2_mapa(mapa, NULL) == ERROR,
                 "Detecta correctamente que el gimnasio es invalido\n");

    destruir_gimnasio(gimnasio);
    destruir_mapa(mapa);
}

void probar_gimnasio_2_mapa_gimnasio_valido () {

    mapa_t* mapa = crear_mapa();
    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_valido.txt";
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio) == EXITO,
                 "Mensaje de exito al agregar un gimnasio al mapa");

    pa2m_afirmar(mapa->gimnasios,
                 "Crea el heap de gimnasios");

    pa2m_afirmar(heap_elementos(mapa->gimnasios) == 1,
                 "La cantidad de gimnasios es 1\n");

    destruir_mapa(mapa);
}

void probar_gimnasio_2_mapa_varios_gimnasios_validos () {

    mapa_t* mapa = crear_mapa();
    char ruta_archivo[MAX_NOMBRE] = "pruebas/gimnasio_formato_valido.txt";
    gimnasio_t* gimnasio_uno = archivo_2_gimnasio (ruta_archivo);
    gimnasio_t* gimnasio_dos = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio_uno) == EXITO,
                 "Mensaje de exito al agregar el primer gimnasio al mapa");

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio_dos) == EXITO,
                 "Mensaje de exito al agregar el segundo gimnasio al mapa");

    pa2m_afirmar(mapa->gimnasios,
                 "Crea el heap de gimnasios");

    pa2m_afirmar(heap_elementos(mapa->gimnasios) == 2,
                 "La cantidad de gimnasios es 2\n");

    destruir_mapa(mapa);
}

void probar_gimnasio_2_mapa () {
    probar_gimnasio_2_mapa_valores_invalidos();
    probar_gimnasio_2_mapa_gimnasio_valido();
    probar_gimnasio_2_mapa_varios_gimnasios_validos();
}

int batalla_prueba (void* pkm_1, void* pkm_2) {
    return (((pokemon_t*)pkm_1)->velocidad > ((pokemon_t*)pkm_2)->velocidad) ? GANO_PRINCIPAL : GANO_ENEMIGO;
}

void probar_batalla_pokemon_valores_invalidos () {
    entrenador_t personaje;
    entrenador_t entrenador;
    funcion_batalla estilo = batalla_prueba;

    pa2m_afirmar(batalla_pokemon(NULL, &entrenador, estilo) == 0,
                 "Reconoce correctamente que el personaje es invalido");

    pa2m_afirmar(batalla_pokemon(&personaje, NULL, estilo) == 0,
                 "Reconoce correctamente que el entrenador es invalido");

    pa2m_afirmar(batalla_pokemon(&personaje, &entrenador, NULL) == 0,
                 "Reconoce correctamente que la funcion de la pelea es invalida\n");

}

void probar_batalla_pokemon_principal_muchos_pokemones () {
    funcion_batalla estilo = batalla_prueba;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_muchos_pokemones.txt";
    entrenador_t* personaje = archivo_2_personaje_principal (ruta_archivo);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal porque tiene mas pokemones\n");

    destruir_entrenador(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_enemigo_muchos_pokemones () {
    funcion_batalla estilo = batalla_prueba;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_pocos_pokemones.txt";
    entrenador_t* personaje = archivo_2_personaje_principal (ruta_archivo);

    strcpy(ruta_archivo, "pruebas/enemigo_muchos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_ENEMIGO,
                "Gana correctamente el enemigo porque tiene mas pokemones\n");

    destruir_entrenador(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_dos_muchos_pokemones () {
    funcion_batalla estilo = batalla_prueba;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_muchos_pokemones.txt";
    entrenador_t* personaje = archivo_2_personaje_principal (ruta_archivo);

    strcpy(ruta_archivo, "pruebas/enemigo_muchos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal aunque la misma cantidad de pokemones, el empieza ganando\n");

    destruir_entrenador(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_dos_pocos_pokemones () {
    funcion_batalla estilo = batalla_prueba;
    char ruta_archivo[MAX_NOMBRE];

    strcpy(ruta_archivo, "pruebas/principal_pocos_pokemones.txt");
    entrenador_t* personaje = archivo_2_personaje_principal (ruta_archivo);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal aunque la misma cantidad de pokemones, el empieza ganando\n");

    destruir_entrenador(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon () {
    probar_batalla_pokemon_valores_invalidos();
    printf("  · El principal tiene muchos pokemones y el enemigo no:\n");
    probar_batalla_pokemon_principal_muchos_pokemones();
    printf("  · El enemigo tiene muchos pokemones y el principal no:\n");
    probar_batalla_pokemon_enemigo_muchos_pokemones();
    printf("  · El principal y el enemigo tiene muchos pokemones:\n");
    probar_batalla_pokemon_dos_muchos_pokemones();
    printf("  · El principal y el enemigo tiene pocos pokemones:\n");
    probar_batalla_pokemon_dos_pocos_pokemones();
}

void probar_tomar_prestado_valores_invalidos() {
    entrenador_t principal;
    int id_pokemon = 5;

    char ruta_archivo[MAX_NOMBRE] = "pruebas/enemigo_pocos_pokemones.txt";
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    entrenador_t* enemigo = (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0);

    pa2m_afirmar(tomar_prestado(NULL, enemigo, id_pokemon) == ERROR,
                 "Reconoce correctamente que el personaje principal es invalido");

    pa2m_afirmar(tomar_prestado(&principal, NULL, id_pokemon) == ERROR,
                 "Reconoce correctamente que el enemigo es invalido");

    pa2m_afirmar(tomar_prestado(&principal, enemigo, id_pokemon) == ERROR,
                 "Reconoce que el id_pokemon es invalido\n");

    destruir_gimnasio(gimnasio);
}

void probar_tomar_prestado_principal_pocos_pokemones () {

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_pocos_pokemones.txt";
    entrenador_t* principal = archivo_2_personaje_principal (ruta_archivo);
    int id_pokemon = 2;

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    entrenador_t* enemigo = (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0);

    pa2m_afirmar(tomar_prestado(principal, enemigo, id_pokemon) == EXITO,
                 "Mensaje de exito al tomar prestado un pokemon");

    pokemon_t* pokemon_prestado = lista_elemento_en_posicion(principal->pokemones, (size_t) ((int) lista_elementos(principal->pokemones) - 1));
    pa2m_afirmar(strcmp("Shellder", pokemon_prestado->nombre) == 0,
                 "Se presto el pokemon correcto ubicada en la ultima posicion");

    pa2m_afirmar(lista_elementos(principal->pokemones) == 4,
                 "Aumento la cantidad de pokemones del personaje principal\n");

    destruir_entrenador(principal);
    destruir_gimnasio(gimnasio);
}

void probar_tomar_prestado_principal_muchos_pokemones () {

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_muchos_pokemones.txt";
    entrenador_t* principal = archivo_2_personaje_principal (ruta_archivo);
    int id_pokemon = 2;

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    gimnasio_t* gimnasio = archivo_2_gimnasio (ruta_archivo);

    entrenador_t* enemigo = (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0);

    pa2m_afirmar(tomar_prestado(principal, enemigo, id_pokemon) == EXITO,
                 "Mensaje de exito al tomar prestado un pokemon");

    pokemon_t* pokemon_prestado = lista_elemento_en_posicion(principal->pokemones, (size_t) MAX_POKEMON_COMBATE);
    pa2m_afirmar(strcmp("Shellder", pokemon_prestado->nombre) == 0,
                 "Se presto el pokemon correcto ubicada como 7mo pokemon");

    pa2m_afirmar(lista_elementos(principal->pokemones) == 8,
                 "Aumento la cantidad de pokemones del personaje principal\n");

    destruir_entrenador(principal);
    destruir_gimnasio(gimnasio);
}

void probar_tomar_prestado () {
    probar_tomar_prestado_valores_invalidos();
    printf("  · El enemigo presta su pokemon cuando el principal tiene pocos:\n");
    probar_tomar_prestado_principal_pocos_pokemones();
    printf("  · El enemigo presta su pokemon cuando el principal tiene muchos:\n");
    probar_tomar_prestado_principal_muchos_pokemones();
}

void probar_reordenar_pokemones_valores_invalido () {

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_muchos_pokemones.txt";
    entrenador_t* principal = archivo_2_personaje_principal (ruta_archivo);
    int pkm_uno = 2, pkm_dos = 3;

    pa2m_afirmar(reordenar_pokemones(NULL, pkm_uno, pkm_dos) == ERROR,
                 "Reconoce que no tiene una lista de pokemones");

    pkm_uno = 60;
    pa2m_afirmar(reordenar_pokemones(principal->pokemones, pkm_uno, pkm_dos) == ERROR,
                 "Reconoce que el id del primer pokemon es invalido");

    pkm_uno = 2;
    pkm_dos = 60;
    pa2m_afirmar(reordenar_pokemones(principal->pokemones, pkm_uno, pkm_dos) == ERROR,
                 "Reconoce que el id del segundo pokemon es invalido\n");

    destruir_entrenador(principal);
}

void probar_reordenar_pokemones_intercambio () {

    char ruta_archivo[MAX_NOMBRE] = "pruebas/principal_muchos_pokemones.txt";
    entrenador_t* principal  = archivo_2_personaje_principal (ruta_archivo);
    int pkm_uno = 1, pkm_dos = 2;

    char pkm_uno_nombre[MAX_NOMBRE], pkm_dos_nombre[MAX_NOMBRE];

    strcpy(pkm_uno_nombre, ((pokemon_t*)lista_elemento_en_posicion(principal->pokemones, (size_t) pkm_uno))->nombre);
    strcpy(pkm_dos_nombre, ((pokemon_t*)lista_elemento_en_posicion(principal->pokemones, (size_t) pkm_dos))->nombre);

    pa2m_afirmar(reordenar_pokemones(principal->pokemones, pkm_uno, pkm_dos) == EXITO,
                 "Mensaje de exito al intercambiar dos pokemones");

    pokemon_t* pokemon = lista_elemento_en_posicion(principal->pokemones, (size_t) pkm_uno);
    pa2m_afirmar(strcmp(pkm_dos_nombre, pokemon->nombre) == 0,
                 "Intercambia correctamente el pokemon");

    pokemon = lista_elemento_en_posicion(principal->pokemones, (size_t) pkm_dos);
    pa2m_afirmar(strcmp(pkm_uno_nombre, pokemon->nombre) == 0,
                 "Intercambia correctamente el pokemon\n");

    destruir_entrenador(principal);
}

void probar_reordenar_pokemones () {
    probar_reordenar_pokemones_valores_invalido();
    probar_reordenar_pokemones_intercambio();
}

int main () {

    pa2m_nuevo_grupo("Pruebas de heap");
    printf(" * Heap crear:\n");
    probar_heap_crear();
    printf("\n * Heap insertar:\n");
    probar_heap_insertar();
    printf("\n * Heap raiz:\n");
    probar_heap_raiz();
    printf("\n * Heap eliminar raiz:\n");
    probar_heap_eliminar_raiz();
    printf("\n * Heap elementos:\n");
    probar_heap_elementos();
    printf("\n * Heap destruir:\n");
    probar_heap_destruir();

    pa2m_nuevo_grupo("Pruebas de herramientas");
    printf(" * De archivo a personaje principal:\n");
    probar_archivo_2_personaje_principal();
    printf("\n * De archivo a gimnasio:\n");
    probar_archivo_2_gimnasio();
    printf("\n * Agregar gimnasio al mapa:\n");
    probar_gimnasio_2_mapa();

    pa2m_nuevo_grupo("Pruebas de batallas");
    printf(" * Batalla pokemon:\n");
    probar_batalla_pokemon();
    printf("\n * Tomar prestado:\n");
    probar_tomar_prestado();
    printf("\n * Reordenar pokemones:\n");
    probar_reordenar_pokemones();

    pa2m_mostrar_reporte();

    return 0;
}
