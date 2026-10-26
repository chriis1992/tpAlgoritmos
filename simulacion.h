#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lru_cache.h"
#include "lru_cache_lista_doble.h"

#define CANT_POSTEOS 50000
#define CANT_USUARIOS_DEFAULT 2000
#define TAM_FEED 20

#define ARCHIVO_POSTEOS "posteos.dat"
#define ARCHIVO_LOTE_CONCENTRADO "usuarios_concentrado.dat"
#define ARCHIVO_LOTE_DISPERSO "usuarios_disperso.dat"
#define ARCHIVO_LOTE_ORDENADO "usuarios_ordenado.dat"
#define ARCHIVO_INFORME "informe_simulacion.txt"

#define TIPO_CONCENTRADO 1
#define TIPO_DISPERSO 2
#define TIPO_ORDENADO 3

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

int comparar_posteos_id(const void *a, const void *b);

int generar_archivo_posteos(const char *rutaPosteos, unsigned cantPosteos);
int generar_lote_usuarios(const char *rutaUsuarios, unsigned cantUsuarios, unsigned cantPosteos, int tipoLote, unsigned semilla);

int leer_posteo_disco(FILE *pfPosteos, int idBuscado, tPosteo *posteoDestino, unsigned *contadorLecturas);

int simular_sin_cache(const char *rutaPosteos, const char *rutaUsuarios, tMetricas *metricas);
int simular_con_cache_parte1(const char *rutaPosteos, const char *rutaUsuarios, unsigned capacidadCache, tMetricas *metricas);
int simular_con_cache_parte2(const char *rutaPosteos, const char *rutaUsuarios, unsigned capacidadCache, tMetricas *metricas);

#endif // SIMULACION_H_INCLUDED
