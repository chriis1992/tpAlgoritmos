#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED

#include <stdlib.h>
#include "lista.h"

typedef struct
{
    tLista lista_elementos;
    unsigned capacidad;
    unsigned cantidad_actual;
    int (*cmp)(const void *, const void *);
} t_lru_cache;

void crear_lrucache(t_lru_cache *cache, unsigned capacidad, int (*cmp)(const void *, const void *));
int obtener_lrucache(t_lru_cache *cache, void *dato, unsigned tam);
int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam);
void vaciar_lrucache(t_lru_cache *cache);

#endif // LRU_CACHE_H_INCLUDED

