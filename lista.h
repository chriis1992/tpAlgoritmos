#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIMO(X,Y) ((X)<(Y)?(X):(Y))

typedef int (*cmp)(const void*, const void*);

typedef struct sNodo
{
    void * info;
    unsigned tamInfo;
    struct sNodo * sig;
} tNodo;

typedef tNodo * tLista;

void crear_lista(tLista * pl);
int poner_primero_lista(tLista * pl, const void * pd, unsigned tam);
int sacar_elem_lista(tLista * pl, void * pd, unsigned tam, cmp cmp);
int sacar_ultimo_lista(tLista * pl, void * pd, unsigned tam);
void vaciar_lista(tLista * pl);
void recorrerLista(tLista * pl, void (*accion)(void*, void*), void * param);

#endif // LISTA_H_INCLUDED
