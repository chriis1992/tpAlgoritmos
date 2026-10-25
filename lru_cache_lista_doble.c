#include "lru_cache_lista_doble.h"

#include <string.h>

void crear_lrucache_lista_doble(t_lru_cache_lista_doble *cache, unsigned capacidad)
{
    crear_lista_doble(&cache->lista);
    cache->capacidadMax = capacidad;
    cache->cantidadActual = 0;
}

int agregar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, const void *dato, unsigned tam, tCmpListaDoble comparar)
{
    tNodoDoble *existente;

    if(cache->capacidadMax == 0 || !dato || tam == 0 || !comparar)
        return 0;

    existente = buscar_nodo_lista_doble(&cache->lista, dato, comparar);
    if(existente)
    {
        if(!reemplazar_nodo_lista_doble(existente, dato, tam))
            return 0;
        mover_primero_lista_doble(&cache->lista, existente);
        return 1;
    }

    if(!poner_primero_lista_doble(&cache->lista, dato, tam))
        return 0;

    cache->cantidadActual++;
    if(cache->cantidadActual > cache->capacidadMax)
    {
        sacar_ultimo_lista_doble(&cache->lista, NULL, 0);
        cache->cantidadActual--;
    }

    return 1;
}

int obtener_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam, tCmpListaDoble comparar)
{
    tNodoDoble *encontrado;

    if(!datoBusqueda || !comparar)
        return 0;

    encontrado = buscar_nodo_lista_doble(&cache->lista, datoBusqueda, comparar);
    if(!encontrado)
        return 0;

    if(tam)
        memcpy(datoBusqueda, encontrado->info, minimo(tam, encontrado->tamInfo));
    mover_primero_lista_doble(&cache->lista, encontrado);
    return 1;
}

int borrar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam, tCmpListaDoble comparar)
{
    tNodoDoble *encontrado;

    if(!datoBusqueda || !comparar)
        return 0;

    encontrado = buscar_nodo_lista_doble(&cache->lista, datoBusqueda, comparar);
    if(!sacar_nodo_lista_doble(&cache->lista, encontrado, datoBusqueda, tam))
        return 0;

    cache->cantidadActual--;
    return 1;
}

void vaciar_lrucache_lista_doble(t_lru_cache_lista_doble *cache)
{
    vaciar_lista_doble(&cache->lista);
    cache->cantidadActual = 0;
}
