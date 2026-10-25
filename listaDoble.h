#ifndef LISTA_DOBLE_H_INCLUDED
#define LISTA_DOBLE_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define minimo(a,b) ((a) <= (b) ? (a) : (b))
#define maximo(a,b) ((a) > (b) ? (a) : (b))

typedef int (*Cmp)(const void *, const void *);

typedef struct sNodoDoble
{
    void *info;
    unsigned tamInfo;
    struct sNodoDoble *ant;
    struct sNodoDoble *sig;
} tNodoDoble;

typedef struct
{
    tNodoDoble *primero;
    tNodoDoble *ultimo;
} tListaDoble;

void crear_lista_doble(tListaDoble *lista);
int poner_primero_lista_doble(tListaDoble *lista, const void *dato, unsigned tam);
tNodoDoble *buscar_nodo_lista_doble(const tListaDoble *lista, const void *clave, Cmp compara);
void mover_primero_lista_doble(tListaDoble *lista, tNodoDoble *nodo);
int reemplazar_nodo_lista_doble(tNodoDoble *nodo, const void *dato, unsigned tam);
int sacar_nodo_lista_doble(tListaDoble *lista, tNodoDoble *nodo, void *destino, unsigned tamDestino);
int sacar_ultimo_lista_doble(tListaDoble *lista, void *destino, unsigned tamDestino);
void vaciar_lista_doble(tListaDoble *lista);

#endif
