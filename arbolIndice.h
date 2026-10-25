#ifndef ARBOL_INDICE_H_INCLUDED
#define ARBOL_INDICE_H_INCLUDED

#include "listaDoble.h"

typedef struct sNodoIndice
{
    tNodoDoble *nodoLista;
    struct sNodoIndice *izq;
    struct sNodoIndice *der;
} tNodoIndice;

typedef tNodoIndice *tArbolIndice;

void crear_arbol_indice(tArbolIndice *arbol);
tNodoDoble *buscar_arbol_indice(const tArbolIndice *arbol, const void *clave, Cmp compara);
int insertar_arbol_indice(tArbolIndice *arbol, tNodoDoble *nodoLista, Cmp compara);
int eliminar_arbol_indice(tArbolIndice *arbol, const void *clave, Cmp compara);
unsigned altura_arbol_indice(const tArbolIndice *arbol);
void vaciar_arbol_indice(tArbolIndice *arbol);

#endif
