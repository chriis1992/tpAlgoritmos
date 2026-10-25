#include "listaDoble.h"

void crear_lista_doble(tListaDoble *lista)
{
    lista->primero = NULL;
    lista->ultimo = NULL;
}

int poner_primero_lista_doble(tListaDoble *lista, const void *dato, unsigned tam)
{
    tNodoDoble *nue;

    if(!dato || tam == 0)
        return 0;

    nue = malloc(sizeof(*nue));
    if(!nue)
        return 0;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info, dato, tam);
    nue->tamInfo = tam;
    nue->ant = NULL;
    nue->sig = lista->primero;

    if(lista->primero)
        lista->primero->ant = nue;
    else
        lista->ultimo = nue;

    lista->primero = nue;
    return 1;
}

tNodoDoble *buscar_nodo_lista_doble(const tListaDoble *lista, const void *clave, Cmp compara)
{
    tNodoDoble *actual;

    if(!clave || !compara)
        return NULL;

    for(actual = lista->primero; actual; actual = actual->sig)
        if(compara(clave, actual->info) == 0)
            return actual;

    return NULL;
}

void mover_primero_lista_doble(tListaDoble *lista, tNodoDoble *nodo)
{
    if(!nodo || nodo == lista->primero)
        return;

    nodo->ant->sig = nodo->sig;
    if(nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        lista->ultimo = nodo->ant;

    nodo->ant = NULL;
    nodo->sig = lista->primero;
    lista->primero->ant = nodo;
    lista->primero = nodo;
}

int reemplazar_nodo_lista_doble(tNodoDoble *nodo, const void *dato, unsigned tam)
{
    void *nuevaInfo;

    if(!nodo || !dato || tam == 0)
        return 0;

    nuevaInfo = malloc(tam);
    if(!nuevaInfo)
        return 0;

    memcpy(nuevaInfo, dato, tam);
    free(nodo->info);
    nodo->info = nuevaInfo;
    nodo->tamInfo = tam;
    return 1;
}

int sacar_nodo_lista_doble(tListaDoble *lista, tNodoDoble *nodo, void *destino, unsigned tamDestino)
{
    if(!nodo)
        return 0;

    if(destino && tamDestino)
        memcpy(destino, nodo->info, minimo(tamDestino, nodo->tamInfo));

    if(nodo->ant)
        nodo->ant->sig = nodo->sig;
    else
        lista->primero = nodo->sig;

    if(nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        lista->ultimo = nodo->ant;

    free(nodo->info);
    free(nodo);
    return 1;
}

int sacar_ultimo_lista_doble(tListaDoble *lista, void *destino, unsigned tamDestino)
{
    return sacar_nodo_lista_doble(lista, lista->ultimo, destino, tamDestino);
}

void vaciar_lista_doble(tListaDoble *lista)
{
    while(lista->ultimo)
        sacar_ultimo_lista_doble(lista, NULL, 0);
}
