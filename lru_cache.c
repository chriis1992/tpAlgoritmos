#include "lru_cache.h"

void crear_lrucache(t_lru_cache *cache, unsigned capacidad, Cmp comparar)
{
    if(!cache)
        return;

    crear_lista(&cache->listaElementos);
    cache->capacidad = capacidad;
    cache->cantidadActual = 0;
    cache->comparar = comparar;
}

int obtener_lrucache(t_lru_cache *cache, void *dato, unsigned tam)
{
    if(!cache || !dato || tam == 0 || !cache->comparar)
        return 0;

    return mover_primero_lista(&cache->listaElementos, dato, tam, cache->comparar);
}

int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam)
{
    if(!cache || cache->capacidad == 0 || !dato || tam == 0 || !cache->comparar)
        return 0;

    if(sacar_elem_lista(&cache->listaElementos, (void *)dato, 0, cache->comparar))
    {
        cache->cantidadActual--;
    }
    else if(cache->cantidadActual >= cache->capacidad)
    {
        sacar_ultimo_lista(&cache->listaElementos, NULL, 0);
        cache->cantidadActual--;
    }

    if(!poner_primero_lista(&cache->listaElementos, dato, tam))
        return 0;

    cache->cantidadActual++;
    return 1;
}

int borrar_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam)
{
    if(!cache || !datoBusqueda || !cache->comparar)
        return 0;

    if(sacar_elem_lista(&cache->listaElementos, datoBusqueda, tam, cache->comparar))
    {
        cache->cantidadActual--;
        return 1;
    }
    return 0;
}

void vaciar_lrucache(t_lru_cache *cache)
{
    if(!cache)
        return;

    vaciar_lista(&cache->listaElementos);
    cache->cantidadActual = 0;
}
