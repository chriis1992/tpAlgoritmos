#ifndef LRU_CACHE_LISTA_DOBLE_H_INCLUDED
#define LRU_CACHE_LISTA_DOBLE_H_INCLUDED

#include "arbolIndice.h"

typedef struct
{
    tListaDoble lista;
    tArbolIndice indice;
    unsigned capacidadMax;
    unsigned cantidadActual;
    Cmp comparar;
} t_lru_cache_lista_doble;

void crear_lrucache_lista_doble(t_lru_cache_lista_doble *cache, unsigned capacidad, Cmp comparar);
int agregar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, const void *dato, unsigned tam);
int obtener_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam);
int borrar_lrucache_lista_doble(t_lru_cache_lista_doble *cache, void *datoBusqueda, unsigned tam);
void vaciar_lrucache_lista_doble(t_lru_cache_lista_doble *cache);
unsigned altura_lrucache_lista_doble(const t_lru_cache_lista_doble *cache);

#endif
