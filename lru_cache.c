#include "lru_cache.h"

void crear_lrucache(t_lru_cache *cache, unsigned capacidad, int (*cmp)(const void *, const void *))
{
    crear_lista(&cache->lista_elementos);
    cache->capacidad = capacidad;
    cache->cantidad_actual = 0;

    cache->cmp = cmp;
}

int obtener_lrucache(t_lru_cache *cache, void *dato, unsigned tam)
{
    if(sacar_elem_lista(&cache->lista_elementos, dato, tam, cache->cmp))
    {
        poner_primero_lista(&cache->lista_elementos, dato, tam);
        return 1;
    }
    return 0;
}

int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam)
{
    if(cache->cantidad_actual >= cache->capacidad)
    {
        sacar_ultimo_lista(&cache->lista_elementos, NULL, tam);
        cache->cantidad_actual--;
    }

    poner_primero_lista(&cache->lista_elementos, dato, tam);
    cache->cantidad_actual++;

    return 1;
}

void vaciar_lrucache(t_lru_cache *cache)
{
    vaciar_lista(&cache->lista_elementos);
    cache->cantidad_actual = 0;
}
