#include "arbolIndice.h"

void crear_arbol_indice(tArbolIndice *arbol)
{
    *arbol = NULL;
}

tNodoDoble *buscar_arbol_indice(const tArbolIndice *arbol, const void *clave, Cmp compara)
{
    int resultado;

    if(!clave || !compara)
        return NULL;

    while(*arbol)
    {
        resultado = compara(clave, (*arbol)->nodoLista->info);
        if(resultado == 0)
            return (*arbol)->nodoLista;
        arbol = resultado < 0 ? &(*arbol)->izq : &(*arbol)->der;
    }
    return NULL;
}

int insertar_arbol_indice(tArbolIndice *arbol, tNodoDoble *nodoLista, Cmp compara)
{
    tNodoIndice *nuevo;
    int resultado;

    if(!nodoLista || !compara)
        return 0;

    while(*arbol)
    {
        resultado = compara(nodoLista->info, (*arbol)->nodoLista->info);
        if(resultado == 0)
            return 0;
        arbol = resultado < 0 ? &(*arbol)->izq : &(*arbol)->der;
    }

    nuevo = malloc(sizeof(*nuevo));
    if(!nuevo)
        return 0;
    nuevo->nodoLista = nodoLista;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    *arbol = nuevo;
    return 1;
}

int eliminar_arbol_indice(tArbolIndice *arbol, const void *clave, Cmp compara)
{
    tNodoIndice *eliminado, **reemplazo, *sucesor;
    int resultado;

    if(!clave || !compara)
        return 0;

    while(*arbol)
    {
        resultado = compara(clave, (*arbol)->nodoLista->info);
        if(resultado == 0)
            break;
        arbol = resultado < 0 ? &(*arbol)->izq : &(*arbol)->der;
    }
    if(!*arbol)
        return 0;

    eliminado = *arbol;
    if(!eliminado->izq)
        *arbol = eliminado->der;
    else if(!eliminado->der)
        *arbol = eliminado->izq;
    else
    {
        /* El sucesor reemplaza la referencia, sin copiar ni liberar datos de la lista. */
        reemplazo = &eliminado->der;
        while((*reemplazo)->izq)
            reemplazo = &(*reemplazo)->izq;
        sucesor = *reemplazo;
        eliminado->nodoLista = sucesor->nodoLista;
        *reemplazo = sucesor->der;
        free(sucesor);
        return 1;
    }

    free(eliminado);
    return 1;
}

unsigned altura_arbol_indice(const tArbolIndice *arbol)
{
    unsigned hi, hd;

    if(!*arbol)
        return 0;
    hi = altura_arbol_indice(&(*arbol)->izq);
    hd = altura_arbol_indice(&(*arbol)->der);
    return maximo(hi, hd) + 1;
}

void vaciar_arbol_indice(tArbolIndice *arbol)
{
    if(!*arbol)
        return;

    vaciar_arbol_indice(&(*arbol)->izq);
    vaciar_arbol_indice(&(*arbol)->der);
    free(*arbol);
    *arbol = NULL;
}
