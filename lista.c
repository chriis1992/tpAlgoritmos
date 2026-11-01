#include "lista.h"

void crear_lista(tLista * pl)
{
    if(pl)
        *pl = NULL;
}

int poner_primero_lista(tLista * pl, const void * pd, unsigned tam)
{
    tNodo * nue;

    if(!pl || !pd || tam == 0)
        return 0;

    nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return 0;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return 0;
    }

    nue->tamInfo = tam;
    memcpy(nue->info, pd, tam);
    nue->sig = *pl;

    *pl = nue;

    return 1;
}

int sacar_elem_lista(tLista * pl, void * pd, unsigned tam, Cmp compara)
{
    tNodo * elim;

    if(!pl || !*pl || !pd || !compara)
        return 0;

    while(*pl && compara(pd, (*pl)->info) != 0)
        pl = &(*pl)->sig;

    if(!*pl)
        return 0;

    elim = *pl;
    if(tam > 0)
        memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));

    *pl = elim->sig;
    free(elim->info);
    free(elim);

    return 1;
}

int sacar_ultimo_lista(tLista * pl, void * pd, unsigned tam)
{
    if(!pl || !*pl)
        return 0;

    while((*pl)->sig)
        pl = &(*pl)->sig;

    tNodo * elim = *pl;

    if(pd && tam > 0)
        memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));

    *pl = NULL;

    free(elim->info);
    free(elim);

    return 1;
}

int mover_primero_lista(tLista * pl, void * pd, unsigned tam, Cmp compara)
{
    tLista * act;
    tNodo * nodo;

    if(!pl || !*pl || !pd || !compara)
        return 0;

    act = pl;
    while(*act && compara(pd, (*act)->info) != 0)
        act = &(*act)->sig;

    if(!*act)
        return 0;

    nodo = *act;
    if(tam > 0)
        memcpy(pd, nodo->info, MINIMO(tam, nodo->tamInfo));

    if(act != pl)
    {
        *act = nodo->sig;
        nodo->sig = *pl;
        *pl = nodo;
    }

    return 1;
}

void vaciar_lista(tLista * pl)
{
    tNodo * elim;

    if(!pl)
        return;

    while(*pl)
    {
        elim = *pl;
        *pl = elim->sig;
        free(elim->info);
        free(elim);
    }
}

void recorrerLista(tLista * pl, void (*accion)(void*, void*), void * param)
{
    if(!pl || !accion)
        return;

    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}

