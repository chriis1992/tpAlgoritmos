#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>

#include "lru_cache.h"
#include "lru_cache_lista_doble.h"

#define CANT_POSTEOS 50000
#define CANT_USUARIOS_DEFAULT 200
#define TAM_FEED 20

#define ARCHIVO_POSTEOS "posteos.dat"
#define ARCHIVO_LOTE_CONCENTRADO "usuarios_concentrado.dat"
#define ARCHIVO_LOTE_DISPERSO "usuarios_disperso.dat"
#define ARCHIVO_LOTE_ORDENADO "usuarios_ordenado.dat"
#define ARCHIVO_INFORME "informe_simulacion.txt"

typedef enum
{
    MODO_SIN_CACHE = 0,
    MODO_CACHE_PARTE_1 = 1,
    MODO_CACHE_PARTE_2 = 2
} tModoCache;

typedef struct
{
    int id;
    char texto[141];
} tPosteo;

typedef struct
{
    char usuario[15];
    int feed[TAM_FEED];
} tUsuario;

typedef struct
{
    double tiempoTotalSeg;
    double tiempoPromUsuarioMs;
    double tiempoPromPosteoMs;
    unsigned cantUsuarios;
    unsigned totalConsultas;
    unsigned aciertos;
    unsigned fallos;
    unsigned lecturasDisco;
    unsigned alturaArbol;
} tMetricas;

/* Operaciones de archivos y posteos */
int abrirArchivo(FILE **fp, const char *nombreArchivo, const char *modoApertura, int mostrarError);
int comparar_posteos_id(const void *a, const void *b);
int generar_archivo_posteos(const char *rutaPosteos, unsigned cantPosteos);
int leer_posteo_disco(FILE *pfPosteos, int id, tPosteo *post, tMetricas *metrica);

/* Generacion de lotes de prueba */
int generar_lote_concentrado(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla);
int generar_lote_disperso(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla);
int generar_lote_ordenado(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla);

/* Procesamiento y mediciones */
void inicializar_metricas(tMetricas *metrica);
int procesar_lote_usuarios(const char *rutaLote, const char *rutaPosteos, tModoCache modo, unsigned capacidadCache, tMetricas *metricas);
void mostrar_metricas(const tMetricas *metricas, const char *nombreLote, tModoCache modo, unsigned capacidad);

#endif // SIMULACION_H_INCLUDED
