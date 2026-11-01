#include "lru_cache_lista_doble.h"

#include <string.h>

void crear_lrucache_lista_doble(t_lru_cache_lista_doble *cache, unsigned capacidad, Cmp comparar)
{
    if(!cache)
        return;

    crear_lista_doble(&cache->lista);
    crear_arbol_indice(&cache->indice);
    cache->capacidadMax = capacidad;
    cache->cantidadActual = 0;
    cache->comparar = comparar;
}

int agregar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, const void *dato, unsigned tam)
{
    tNodoDoble *existente;

    if(!cache || cache->capacidadMax == 0 || !dato || tam == 0 || !cache->comparar)
        return 0;

    existente = buscar_arbol_indice(&cache->indice, dato, cache->comparar);
    if(existente)
    {
        if(!reemplazar_nodo_lista_doble(existente, dato, tam))
            return 0;
        mover_primero_lista_doble(&cache->lista, existente);
        return 1;
    }

    if(!poner_primero_lista_doble(&cache->lista, dato, tam))
        return 0;

    if(!insertar_arbol_indice(&cache->indice, cache->lista.primero, cache->comparar))
    {
        sacar_nodo_lista_doble(&cache->lista, cache->lista.primero, NULL, 0);
        return 0;
    }

    if(cache->cantidadActual >= cache->capacidadMax)
    {
        /* saco el nodo primero del indice y despues se saca de la lista */
        eliminar_arbol_indice(&cache->indice, cache->lista.ultimo->info, cache->comparar);
        sacar_ultimo_lista_doble(&cache->lista, NULL, 0);
        cache->cantidadActual = cache->capacidadMax;
    }
    else
        cache->cantidadActual++;

    return 1;
}

int obtener_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam)
{
    tNodoDoble *encontrado;

    if(!cache || !datoBusqueda || !cache->comparar)
        return 0;

    encontrado = buscar_arbol_indice(&cache->indice, datoBusqueda, cache->comparar);
    if(!encontrado)
        return 0;

    if(tam)
        memcpy(datoBusqueda, encontrado->info, minimo(tam, encontrado->tamInfo));
    mover_primero_lista_doble(&cache->lista, encontrado);
    return 1;
}

int borrar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam)
{
    tNodoDoble *encontrado;

    if(!cache || !datoBusqueda || !cache->comparar)
        return 0;

    encontrado = buscar_arbol_indice(&cache->indice, datoBusqueda, cache->comparar);
    if(!encontrado)
        return 0;

    eliminar_arbol_indice(&cache->indice, encontrado->info, cache->comparar);
    sacar_nodo_lista_doble(&cache->lista, encontrado, datoBusqueda, tam);
    cache->cantidadActual--;
    return 1;
}

void vaciar_lrucache_lista_doble(t_lru_cache_lista_doble *cache)
{
    if(!cache)
        return;

    vaciar_arbol_indice(&cache->indice);
    vaciar_lista_doble(&cache->lista);
    cache->cantidadActual = 0;
}

unsigned altura_lrucache_lista_doble(const t_lru_cache_lista_doble *cache)
{
    if(!cache)
        return 0;

    return altura_arbol_indice(&cache->indice);
}
