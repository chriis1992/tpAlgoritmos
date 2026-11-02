/*
 * Archivo de pruebas unitarias para LRU Cache (Parte 1 y Parte 2).
 *
 * Para compilar y ejecutar de forma independiente:
 * gcc -Wall -Wextra lista.c listaDoble.c arbolIndice.c lru_cache.c lru_cache_lista_doble.c test_lru.c -o test_lru.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "lru_cache.h"
#include "lru_cache_lista_doble.h"

typedef struct
{
    int clave;
    char valor[32];
} tElementoPrueba;

int comparar_elementos(const void *a, const void *b)
{
    const tElementoPrueba *ea = (const tElementoPrueba *)a;
    const tElementoPrueba *eb = (const tElementoPrueba *)b;
    return (ea->clave > eb->clave) - (ea->clave < eb->clave);
}

void test_lru_cache_parte_1(void)
{
    printf("=== Testeando LRU Cache Parte 1 (Lista Simple) ===\n");
    t_lru_cache cache;
    tElementoPrueba elem, aux;

    crear_lrucache(&cache, 3, comparar_elementos);
    assert(cache.cantidadActual == 0);

    /* 1. Agregar 3 elementos (clave: 1, 2, 3) */
    elem.clave = 1; sprintf(elem.valor, "Uno");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);

    elem.clave = 2; sprintf(elem.valor, "Dos");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);

    elem.clave = 3; sprintf(elem.valor, "Tres");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    /* En este momento el orden MRU -> LRU es: 3, 2, 1 */

    /* 2. Acceder al elemento 1: debe moverse al frente. Orden: 1, 3, 2 */
    aux.clave = 1;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno") == 0);

    /* 3. Agregar elemento 4: debe desbordar y eliminar el LRU (que ahora es el 2) */
    elem.clave = 4; sprintf(elem.valor, "Cuatro");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    /* El 2 no debe existir */
    aux.clave = 2;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* Los elementos 4, 1, 3 si deben existir */
    aux.clave = 4;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Cuatro") == 0);

    aux.clave = 1;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno") == 0);

    aux.clave = 3;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Tres") == 0);

    /* 4. Reemplazo de clave existente: no debe desbordar ni aumentar cantidad */
    elem.clave = 1; sprintf(elem.valor, "Uno Modificado");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    aux.clave = 1;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno Modificado") == 0);

    /* 5. Borrar un elemento */
    aux.clave = 4;
    assert(borrar_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 2);
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* 6. Vaciar cache */
    vaciar_lrucache(&cache);
    assert(cache.cantidadActual == 0);
    aux.clave = 1;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* 7. Casos borde: capacidad 0 y capacidad 1 */
    t_lru_cache cache_cero;
    crear_lrucache(&cache_cero, 0, comparar_elementos);
    assert(agregar_lrucache(&cache_cero, &elem, sizeof(tElementoPrueba)) == 0);

    crear_lrucache(&cache, 1, comparar_elementos);
    elem.clave = 10; sprintf(elem.valor, "Diez");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 1);

    elem.clave = 20; sprintf(elem.valor, "Veinte");
    assert(agregar_lrucache(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 1);

    aux.clave = 10;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 0);
    aux.clave = 20;
    assert(obtener_lrucache(&cache, &aux, sizeof(tElementoPrueba)) == 1);

    vaciar_lrucache(&cache);

    printf("-> Parte 1 PASO todas las pruebas exitosamente!\n\n");
}

void test_lru_cache_parte_2(void)
{
    printf("=== Testeando LRU Cache Parte 2 (Lista Doble + Arbol Indice) ===\n");
    t_lru_cache_lista_doble cache;
    tElementoPrueba elem, aux;

    crear_lrucache_lista_doble(&cache, 3, comparar_elementos);
    assert(cache.cantidadActual == 0);
    assert(altura_lrucache_lista_doble(&cache) == 0);

    /* 1. Agregar 3 elementos (clave: 1, 2, 3) */
    elem.clave = 1; sprintf(elem.valor, "Uno");
    assert(agregar_lrucache_lista_doble(&cache, &elem, sizeof(tElementoPrueba)) == 1);

    elem.clave = 2; sprintf(elem.valor, "Dos");
    assert(agregar_lrucache_lista_doble(&cache, &elem, sizeof(tElementoPrueba)) == 1);

    elem.clave = 3; sprintf(elem.valor, "Tres");
    assert(agregar_lrucache_lista_doble(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    /* Al insertar en orden creciente 1, 2, 3 en el ABB, la altura debe ser 3 */
    assert(altura_lrucache_lista_doble(&cache) == 3);

    /* En este momento el orden MRU -> LRU en la lista es: 3, 2, 1 */

    /* 2. Acceder al elemento 1: debe moverse al frente de la lista. Orden: 1, 3, 2 */
    aux.clave = 1;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno") == 0);

    /* 3. Agregar elemento 4: debe desbordar y eliminar el LRU (que ahora es el 2) */
    elem.clave = 4; sprintf(elem.valor, "Cuatro");
    assert(agregar_lrucache_lista_doble(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    /* El 2 no debe existir ni en lista ni en arbol */
    aux.clave = 2;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* Los elementos 4, 1, 3 si deben existir */
    aux.clave = 4;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Cuatro") == 0);

    aux.clave = 1;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno") == 0);

    aux.clave = 3;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Tres") == 0);

    /* 4. Reemplazo de clave existente: no debe desbordar ni aumentar cantidad */
    elem.clave = 1; sprintf(elem.valor, "Uno Modificado");
    assert(agregar_lrucache_lista_doble(&cache, &elem, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 3);

    aux.clave = 1;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(strcmp(aux.valor, "Uno Modificado") == 0);

    /* 5. Borrar un elemento */
    aux.clave = 4;
    assert(borrar_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 2);
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* 6. Vaciar cache */
    vaciar_lrucache_lista_doble(&cache);
    assert(cache.cantidadActual == 0);
    assert(altura_lrucache_lista_doble(&cache) == 0);
    aux.clave = 1;
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* 7. Caso especial: Eliminacion de nodo con 2 hijos en el Arbol Indice */
    crear_lrucache_lista_doble(&cache, 10, comparar_elementos);
    elem.clave = 10; sprintf(elem.valor, "Diez"); agregar_lrucache_lista_doble(&cache, &elem, sizeof(elem));
    elem.clave = 5; sprintf(elem.valor, "Cinco"); agregar_lrucache_lista_doble(&cache, &elem, sizeof(elem));
    elem.clave = 15; sprintf(elem.valor, "Quince"); agregar_lrucache_lista_doble(&cache, &elem, sizeof(elem));
    elem.clave = 12; sprintf(elem.valor, "Doce"); agregar_lrucache_lista_doble(&cache, &elem, sizeof(elem));
    elem.clave = 20; sprintf(elem.valor, "Veinte"); agregar_lrucache_lista_doble(&cache, &elem, sizeof(elem));

    /* Borramos el 15 (tiene 2 hijos: 12 y 20) */
    aux.clave = 15;
    assert(borrar_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    assert(cache.cantidadActual == 4);
    assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 0);

    /* Verificar que los demas siguen accesibles */
    aux.clave = 10; assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    aux.clave = 5;  assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    aux.clave = 12; assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);
    aux.clave = 20; assert(obtener_lrucache_lista_doble(&cache, &aux, sizeof(tElementoPrueba)) == 1);

    vaciar_lrucache_lista_doble(&cache);

    printf("-> Parte 2 PASO todas las pruebas exitosamente!\n\n");
}

int main(void)
{
    test_lru_cache_parte_1();
    test_lru_cache_parte_2();

    printf("Todas las pruebas de estructuras y caches finalizaron con EXITO.\n");
    return 0;
}
