#ifndef LRU_CACHE_LISTA_DOBLE_H_INCLUDED
#define LRU_CACHE_LISTA_DOBLE_H_INCLUDED

#include "listaDoble.h"

typedef struct
{
    tListaDoble lista;
    unsigned capacidadMax;
    unsigned cantidadActual;
} t_lru_cache_lista_doble;

void crear_lrucache_lista_doble(t_lru_cache_lista_doble *cache, unsigned capacidad);
int agregar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, const void *dato, unsigned tam, tCmpListaDoble comparar);
int obtener_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam, tCmpListaDoble comparar);
int borrar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam, tCmpListaDoble comparar);
void vaciar_lrucache_lista_doble(t_lru_cache_lista_doble *cache);

#endif
