#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED

#include <stdlib.h>
#include "lista.h"

typedef struct
{
    tLista listaElementos;
    unsigned capacidad;
    unsigned cantidadActual;
    Cmp comparar;
} t_lru_cache;

void crear_lrucache(t_lru_cache *cache, unsigned capacidad, Cmp comparar);
int obtener_lrucache(t_lru_cache *cache, void *dato, unsigned tam);
int agregar_lrucache(t_lru_cache *cache, const void *dato, unsigned tam);
int borrar_lrucache(t_lru_cache *cache, void *datoBusqueda, unsigned tam);
void vaciar_lrucache(t_lru_cache *cache);

#endif // LRU_CACHE_H_INCLUDED

