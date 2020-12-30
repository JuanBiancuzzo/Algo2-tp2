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

    pa2m_afirmar(heap_insertar(heap, &elemento_tres) == EXITO,
                 "Mensaje de exito al insertar el primer elemento");

    pa2m_afirmar(heap_insertar(heap, &elemento_dos) == EXITO,
                 "Mensaje de exito al insertar el segunfo elemento");

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
                 "Devuelve el valor correcto al eliminar con el heap de un elemento");

    pa2m_afirmar(heap->cant_elementos == 0,
                 "Vuelve el contador a 0 despues de eliminar con el heap de un elemento\n");

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
                 "Devuelve el valor correcto al eliminar con el heap de 3 elementos");

    pa2m_afirmar(heap->cant_elementos == 2,
                 "Vuelve el contador a 2 despues de eliminar con el heap de 3 elementos");

    pa2m_afirmar(elemento_dos == *(int*)heap->elementos[0],
                 "Actualiza correctamente la estructura despues de sacar la raiz\n");

    heap_destruir(heap);
}

void probar_heap_raiz () {
    probar_heap_raiz_valores_invalidos();
    probar_heap_raiz_con_un_elemento();
    probar_heap_raiz_con_varios_elementos();
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

    char ruta_archivo[MAX_NOMBRE];
    personaje_t personaje;

    pa2m_afirmar(archivo_2_personaje_principal(NULL, &personaje) == ERROR,
                 "Detecta correctamente que la ruta del archivo es invalido");

    pa2m_afirmar(archivo_2_personaje_principal(ruta_archivo, NULL) == ERROR,
                 "Detecta correctamente que la direccion del personaje es invalido");

    pa2m_afirmar(archivo_2_personaje_principal("algo_random.txt", &personaje) == ERROR,
                 "Detecta correctamente que la ruta del archivo no lleva a ningun lado\n");

}

void probar_archivo_2_personaje_principal_formato_invalidos_sin_entrenado () {

    personaje_t* personaje = crear_personaje_principal();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/per_principal_formato_invalido_sin_entrenador.txt");

    pa2m_afirmar(archivo_2_personaje_principal(ruta_archivo, personaje) == ERROR,
                 "Reconoce que no hay entrenador y manda un error\n");

    destruir_personaje_principal(personaje);
}

void probar_archivo_2_personaje_principal_formato_invalidos_sin_pokemones () {

    personaje_t* personaje = crear_personaje_principal();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/per_principal_formato_invalido_sin_pokemones.txt");

    pa2m_afirmar(archivo_2_personaje_principal(ruta_archivo, personaje) == ERROR,
                 "Reconoce que no hay pokemones y manda un error\n");

    destruir_personaje_principal(personaje);
}

void probar_archivo_2_personaje_principal_formato_invalidos_corrupto () {

    personaje_t* personaje = crear_personaje_principal();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/per_principal_formato_invalido_corrupto.txt");

    pa2m_afirmar(archivo_2_personaje_principal(ruta_archivo, personaje) == EXITO,
                 "Mensaje de exito en un archivo donde en el medio de los pokemones hay un error");

    pa2m_afirmar(personaje->cant_pokemones == 2,
                 "La cantidad de pokemones es 2 al encontrar el tercer pokemon con un error\n");

    destruir_personaje_principal(personaje);
}

void probar_archivo_2_personaje_principal_formato_validos () {

    personaje_t* personaje = crear_personaje_principal();
    int cant_pokemones = 11;

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/per_principal_formato_valido.txt");

    pa2m_afirmar(archivo_2_personaje_principal(ruta_archivo, personaje) == EXITO,
                 "Mensaje de exito al convertir el archivo a personaje");

    pa2m_afirmar(strcmp(personaje->nombre, "Ash") == 0,
                 "Reconoce correctamente el nombre del personaje principal");

    pa2m_afirmar(personaje->cant_pokemones == cant_pokemones,
                 "Reconoce la cantidad correcta de pokemones");

    pa2m_afirmar(personaje->pokemones,
                 "Crea una cola con los pokemones");

    pa2m_afirmar((int)lista_elementos(personaje->pokemones) == cant_pokemones,
                 "La cola tiene la cantidad correcta de pokemones\n");

    destruir_personaje_principal(personaje);
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

    char ruta_archivo[MAX_NOMBRE];
    gimnasio_t gimnasio;

    pa2m_afirmar(archivo_2_gimnasio(NULL, &gimnasio) == ERROR,
                 "Detecta correctamente que la ruta del archivo es invalido");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, NULL) == ERROR,
                 "Detecta correctamente que la direccion del gimnasio es invalido");

    pa2m_afirmar(archivo_2_gimnasio("algo_random.txt", &gimnasio) == ERROR,
                 "Detecta correctamente que la ruta del archivo no lleva a ningun lado\n");

}

void probar_archivo_2_gimnasio_formato_invalidos_sin_gimnasio () {

    gimnasio_t* gimnasio = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_invalido_sin_gimnasio.txt");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, gimnasio) == ERROR,
                 "Reconoce que no hay gimnasio y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_sin_lider () {

    gimnasio_t* gimnasio = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_invalido_sin_lider.txt");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, gimnasio) == ERROR,
                 "Reconoce que el gimnasio no tiene lider y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_lider_sin_pokemon () {

    gimnasio_t* gimnasio = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_invalido_lider_sin_pokemon.txt");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, gimnasio) == ERROR,
                 "Reconoce que el lider no tiene pokemones, por lo que no tiene un lider valido y manda un error\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_invalidos_entrenador_sin_pokemon () {

    gimnasio_t* gimnasio = crear_gimnasio();
    int cant_entrenadores = 2;

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_invalido_entrenador_sin_pokemon.txt");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, gimnasio) == EXITO,
                 "Aunque haya un entrenador invalido, lo ignora y sigue leyendo");

    pa2m_afirmar(gimnasio->cant_entrenadores == cant_entrenadores,
                 "Devuelve la cantidad correcta de entrenadores\n");

    destruir_gimnasio(gimnasio);
}

void probar_archivo_2_gimnasio_formato_validos () {

    gimnasio_t* gimnasio = crear_gimnasio();
    int cant_entrenadores = 3;

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_valido.txt");

    pa2m_afirmar(archivo_2_gimnasio(ruta_archivo, gimnasio) == EXITO,
                 "Mensaje de exito al convertir el archivo a un gimnasio");

    pa2m_afirmar(strcmp(gimnasio->nombre, "Gimnasio de Ciudad Celeste") == 0,
                 "Reconoce correctamente el nombre del gimnasio");

    pa2m_afirmar(gimnasio->cant_entrenadores == cant_entrenadores,
                 "Reconoce la cantidad correcta de entrenadores");

    pa2m_afirmar(gimnasio->entrenadores,
                 "Crea una pila con los entrenadores");

    pa2m_afirmar((int)lista_elementos(gimnasio->entrenadores) == cant_entrenadores,
                 "La pila tiene la cantidad correcta de entrenadores\n");

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

    gimnasio_t* gimnasio = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_valido.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(gimnasio_2_mapa(NULL, gimnasio) == ERROR,
                 "Detecta correctamente que el mapa es invalido");

    pa2m_afirmar(gimnasio_2_mapa(mapa, NULL) == ERROR,
                 "Detecta correctamente que el gimnasio es invalido\n");

    destruir_gimnasio(gimnasio);
    destruir_mapa(mapa);
}

void probar_gimnasio_2_mapa_gimnasio_valido () {
     mapa_t* mapa = crear_mapa();

    gimnasio_t* gimnasio = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_valido.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio) == EXITO,
                 "Mensaje de exito al agregar un gimnasio al mapa");

    pa2m_afirmar(mapa->gimnasios,
                 "Crea el heap de gimnasios");

    pa2m_afirmar(mapa->cant_gimnasios == 1,
                 "La cantidad de gimnasios es 1\n");

    destruir_mapa(mapa);
}

void probar_gimnasio_2_mapa_varios_gimnasios_validos () {
     mapa_t* mapa = crear_mapa();

    gimnasio_t* gimnasio_uno = crear_gimnasio();
    gimnasio_t* gimnasio_dos = crear_gimnasio();

    char ruta_archivo[MAX_NOMBRE];
    strcpy(ruta_archivo, "pruebas/gimnasio_formato_valido.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio_uno);
    archivo_2_gimnasio (ruta_archivo, gimnasio_dos);

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio_uno) == EXITO,
                 "Mensaje de exito al agregar el primer gimnasio al mapa");

    pa2m_afirmar(gimnasio_2_mapa(mapa, gimnasio_dos) == EXITO,
                 "Mensaje de exito al agregar el segundo gimnasio al mapa");

    pa2m_afirmar(mapa->gimnasios,
                 "Crea el heap de gimnasios");

    pa2m_afirmar(mapa->cant_gimnasios == 2,
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
    personaje_t personaje;
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
    personaje_t* personaje = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    funcion_batalla estilo = batalla_prueba;
    char ruta_archivo[MAX_NOMBRE];

    strcpy(ruta_archivo, "pruebas/principal_muchos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, personaje);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal porque tiene mas pokemones\n");

    destruir_personaje_principal(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_enemigo_muchos_pokemones () {
    personaje_t* personaje = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    funcion_batalla estilo = batalla_prueba;
    char ruta_archivo[MAX_NOMBRE];

    strcpy(ruta_archivo, "pruebas/principal_pocos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, personaje);

    strcpy(ruta_archivo, "pruebas/enemigo_muchos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_ENEMIGO,
                "Gana correctamente el enemigo porque tiene mas pokemones\n");

    destruir_personaje_principal(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_dos_muchos_pokemones () {
    personaje_t* personaje = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    funcion_batalla estilo = batalla_prueba;
    char ruta_archivo[MAX_NOMBRE];

    strcpy(ruta_archivo, "pruebas/principal_muchos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, personaje);

    strcpy(ruta_archivo, "pruebas/enemigo_muchos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal aunque la misma cantidad de pokemones, el empieza ganando\n");

    destruir_personaje_principal(personaje);
    destruir_gimnasio(gimnasio);
}

void probar_batalla_pokemon_dos_pocos_pokemones () {
    personaje_t* personaje = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    funcion_batalla estilo = batalla_prueba;
    char ruta_archivo[MAX_NOMBRE];

    strcpy(ruta_archivo, "pruebas/principal_pocos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, personaje);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    pa2m_afirmar(batalla_pokemon(personaje, (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0), estilo) == GANO_PRINCIPAL,
                "Gana correctamente el personaje principal aunque la misma cantidad de pokemones, el empieza ganando\n");

    destruir_personaje_principal(personaje);
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
    gimnasio_t* gimnasio = crear_gimnasio();
    char ruta_archivo[MAX_NOMBRE];
    personaje_t principal;
    int id_pokemon = 5;

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

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
    personaje_t* principal = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    char ruta_archivo[MAX_NOMBRE];
    int id_pokemon = 2;

    strcpy(ruta_archivo, "pruebas/principal_pocos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, principal);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    entrenador_t* enemigo = (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0);

    pa2m_afirmar(tomar_prestado(principal, enemigo, id_pokemon) == EXITO,
                 "Mensaje de exito al tomar prestado un pokemon");

    pokemon_t* pokemon_prestado = lista_elemento_en_posicion(principal->pokemones, (size_t) (principal->cant_pokemones - 1));
    pa2m_afirmar(strcmp("Shellder", pokemon_prestado->nombre) == 0,
                 "Se presto el pokemon correcto ubicada en la ultima posicion");

    pa2m_afirmar(principal->cant_pokemones == 4,
                 "Aumento la cantidad de pokemones del personaje principal\n");

    destruir_personaje_principal(principal);
    destruir_gimnasio(gimnasio);
}

void probar_tomar_prestado_principal_muchos_pokemones () {
    personaje_t* principal = crear_personaje_principal();
    gimnasio_t* gimnasio = crear_gimnasio();
    char ruta_archivo[MAX_NOMBRE];
    int id_pokemon = 2;

    strcpy(ruta_archivo, "pruebas/principal_muchos_pokemones.txt");
    archivo_2_personaje_principal (ruta_archivo, principal);

    strcpy(ruta_archivo, "pruebas/enemigo_pocos_pokemones.txt");
    archivo_2_gimnasio (ruta_archivo, gimnasio);

    entrenador_t* enemigo = (entrenador_t*)lista_elemento_en_posicion(gimnasio->entrenadores, 0);

    pa2m_afirmar(tomar_prestado(principal, enemigo, id_pokemon) == EXITO,
                 "Mensaje de exito al tomar prestado un pokemon");

    pokemon_t* pokemon_prestado = lista_elemento_en_posicion(principal->pokemones, (size_t) MAX_POKE_COMBATE);
    pa2m_afirmar(strcmp("Shellder", pokemon_prestado->nombre) == 0,
                 "Se presto el pokemon correcto ubicada como 7mo pokemon");

    pa2m_afirmar(principal->cant_pokemones == 8,
                 "Aumento la cantidad de pokemones del personaje principal\n");

    destruir_personaje_principal(principal);
    destruir_gimnasio(gimnasio);
}

void probar_tomar_prestado () {
    probar_tomar_prestado_valores_invalidos();
    printf("  · El enemigo presta su pokemon cuando el principal tiene pocos:\n");
    probar_tomar_prestado_principal_pocos_pokemones();
    printf("  · El enemigo presta su pokemon cuando el principal tiene muchos:\n");
    probar_tomar_prestado_principal_muchos_pokemones();
}

void probar_level_up_valor_invalido () {
    pokemon_t pokemon;

    pokemon.velocidad = MAX_LEVEL_UP;
    pokemon.defensa = MAX_LEVEL_UP;
    pokemon.ataque = MAX_LEVEL_UP;

    pa2m_afirmar(level_up(NULL) == ERROR,
                 "Reconoce correctamente que el pokemon es invalido");

    pa2m_afirmar(pokemon.velocidad == MAX_LEVEL_UP,
                 "Detecta que la velocidad llego a su maximo");

    pa2m_afirmar(pokemon.defensa == MAX_LEVEL_UP,
                 "Detecta que la defensa llego a su maximo");

    pa2m_afirmar(pokemon.ataque == MAX_LEVEL_UP,
                 "Detecta que el ataque llego a su maximo\n");

}

void probar_level_up_valor_valido () {
    pokemon_t pokemon;

    pokemon.velocidad = 11;
    pokemon.defensa = 13;
    pokemon.ataque = 15;

    pa2m_afirmar(level_up(&pokemon) == EXITO,
                 "Mensaje de exito al level up el pokemon");

    pa2m_afirmar(pokemon.velocidad == 12,
                 "Aumenta correctamente la velocidad");

    pa2m_afirmar(pokemon.defensa == 14,
                 "Aumenta correctamente la defensa");

    pa2m_afirmar(pokemon.ataque == 16,
                 "Aumenta correctamente el ataque\n");

}

void probar_level_up () {
    probar_level_up_valor_invalido();
    probar_level_up_valor_valido();
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
    printf("\n * Level up:\n");
    probar_level_up();

    pa2m_mostrar_reporte();

    return 0;
}
