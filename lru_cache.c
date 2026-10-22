#include "lru_cache.h"

void crear_lrucache(t_lru_cache *cache, unsigned capacidad)
{
    crear_lista(&cache->lista);
    cache->capacidadMax = capacidad;
    cache->cantidadActual = 0;
}

int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam, cmp cmp)
{
    void *aux;

    if(cache->capacidadMax == 0)
        return 0;

    aux = malloc(tam);

    if(!aux)
        return 0;

    memcpy(aux, dato, tam);

    if(sacar_elem_lista(&cache->lista, aux, tam, cmp))
    {
        cache->cantidadActual--;
    }
    else
    {
        if(cache->cantidadActual >= cache->capacidadMax)
        {
            if(sacar_ultimo_lista(&cache->lista, aux, tam))
                cache->cantidadActual--;
        }
    }

    free(aux);

    if(!poner_primero_lista(&cache->lista, dato, tam))
        return 0;

    cache->cantidadActual++;

    return 1;
}

int obtener_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam, cmp cmp)
{
    if(sacar_elem_lista(&cache->lista, datoBusqueda, tam, cmp))
    {
        poner_primero_lista(&cache->lista, datoBusqueda, tam);
        return 1;
    }
    return 0;
}

int borrar_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam, cmp cmp)
{
    if(sacar_elem_lista(&cache->lista, datoBusqueda, tam, cmp))
    {
        cache->cantidadActual--;
        return 1;
    }
    return 0;
}

void vaciar_lrucache(t_lru_cache *cache)
{
    vaciar_lista(&cache->lista);
    cache->cantidadActual = 0;
}
