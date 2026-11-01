#include "simulacion.h"

int abrirArchivo(FILE **fp, const char *nombreArchivo, const char *modoApertura, int mostrarError)
{
    *fp = fopen(nombreArchivo, modoApertura);

    if(*fp == NULL)
    {
        if(mostrarError == 1)
        {
            fprintf(stderr, "Error abriendo \"%s\" en modo \"%s\".\n", nombreArchivo, modoApertura);
        }
        return 0;
    }
    return 1;
}

void inicializar_metricas(tMetricas *metrica)
{
    metrica->tiempoTotalSeg = 0.0;
    metrica->tiempoPromUsuarioMs = 0.0;
    metrica->tiempoPromPosteoMs = 0.0;
    metrica->cantUsuarios = 0;
    metrica->totalConsultas = 0;
    metrica->aciertos = 0;
    metrica->fallos = 0;
    metrica->lecturasDisco = 0;
    metrica->alturaArbol = 0;
}

int comparar_posteos_id(const void *a, const void *b)
{
    const tPosteo *p1 = (const tPosteo *)a;
    const tPosteo *p2 = (const tPosteo *)b;
    return p1->id - p2->id;
}

int generar_archivo_posteos(const char *rutaPosteos, unsigned cantPosteos)
{
    FILE *pf;
    int i;
    tPosteo post;

    if(!abrirArchivo(&pf, rutaPosteos, "wb", 1))
        return 0;

    for(i=0;i<cantPosteos;i++)
    {
        post.id = i;
        sprintf(post.texto, "Posteo %d prueba de LRU Cache.", i);
        fwrite(&post, sizeof(tPosteo), 1, pf);
    }

    fclose(pf);
    return 1;
}
