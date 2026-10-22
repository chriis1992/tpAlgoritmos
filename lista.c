#include "Lista.h"

void crear_lista(tLista * pl)
{
    *pl = NULL;
}

int poner_primero_lista(tLista * pl, const void * pd, unsigned tam)
{
    tNodo * nue = (tNodo*)malloc(sizeof(tNodo));

    if(!nue)
    {
        return 0;
    }

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


int sacar_elem_lista(tLista * pl, void * pd, unsigned tam, cmp cmp)
{
    tNodo * elim;

    while(*pl && cmp(pd, (*pl)->info) != 0)
        pl = &(*pl)->sig;

    if(!*pl)
        return 0;

    elim = *pl;
    memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));

    *pl = elim->sig;
    free(elim->info);
    free(elim);

    return 1;
}

int sacar_ultimo_lista(tLista * pl, void * pd, unsigned tam)
{
    if(!*pl)
    {
        return 0;
    }

    while((*pl)->sig)
    {
        pl = &(*pl)->sig;
    }

    tNodo * elim = *pl;

    if(pd)
    {
        memcpy(pd, elim->info, MINIMO(tam, elim->tamInfo));
    }

    *pl = NULL;

    free(elim->info);
    free(elim);

    return 1;
}

void vaciar_lista(tLista * pl)
{
    tNodo * elim;
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
    while(*pl)
    {
        accion((*pl)->info, param);
        pl = &(*pl)->sig;
    }
}

