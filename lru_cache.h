#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED

#include "lista.h"

typedef int (*cmp)(const void*, const void*);

typedef struct
{
    tLista lista;
    unsigned capacidadMax;
    unsigned cantidadActual;
} t_lru_cache;

void crear_lrucache(t_lru_cache *cache, unsigned capacidad);

int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam, cmp cmp);

int obtener_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam, cmp cmp);

int borrar_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam, cmp cmp);

void vaciar_lrucache(t_lru_cache *cache);

#endif // LRU_CACHE_H_INCLUDED
