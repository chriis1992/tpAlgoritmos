#include "lru_cache.h"

void crear_lrucache(t_lru_cache *cache, unsigned capacidad, int (*cmp)(const void *, const void *))
{
    crear_lista(&cache->listaElementos);
    cache->capacidad = capacidad;
    cache->cantidadActual = 0;

    cache->cmp = cmp;
}

int obtener_lrucache(t_lru_cache *cache, void *dato, unsigned tam)
{
    if(sacar_elem_lista(&cache->listaElementos, dato, tam, cache->cmp))
    {
        poner_primero_lista(&cache->listaElementos, dato, tam);
        return 1;
    }
    return 0;
}

int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam)
{
    if(cache->cantidadActual >= cache->capacidad)
    {
        sacar_ultimo_lista(&cache->listaElementos, NULL, tam);
        cache->cantidadActual--;
    }

    poner_primero_lista(&cache->listaElementos, dato, tam);
    cache->cantidadActual++;

    return 1;
}

void vaciar_lrucache(t_lru_cache *cache)
{
    vaciar_lista(&cache->listaElementos);
    cache->cantidadActual = 0;
}
