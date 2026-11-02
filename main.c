#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simulacion.h"

void imprimir_ayuda(const char *prog)
{
    printf("Uso por linea de comandos:\n");
    printf("  %s 1                                    -> Generar archivo de posteos (%s)\n", prog, ARCHIVO_POSTEOS);
    printf("  %s 2 [semilla]                          -> Generar lote concentrado (%s)\n", prog, ARCHIVO_LOTE_CONCENTRADO);
    printf("  %s 3 [semilla]                          -> Generar lote disperso (%s)\n", prog, ARCHIVO_LOTE_DISPERSO);
    printf("  %s 4 [semilla]                          -> Generar lote ordenado (%s)\n", prog, ARCHIVO_LOTE_ORDENADO);
    printf("  %s 5 <ruta_lote> <modo:0|1|2> <cap>     -> Probar rendimiento\n", prog);
    printf("         modo: 0 = Sin cache, 1 = Parte 1 (Lista), 2 = Parte 2 (Doble+ABB)\n");
    printf("  %s 6 [semilla]                          -> Ejecutar bateria completa de pruebas para el informe\n\n", prog);
}

void ejecutar_bateria_informe(unsigned semilla)
{
    const unsigned capacidades[] = { 10, 50, 100, 200, 500, 1000, 2000 };
    const size_t cantCapacidades = sizeof(capacidades) / sizeof(capacidades[0]);
    tMetricas m;

    printf("\n=== GENERANDO ARCHIVOS PARA LA BATERIA DE PRUEBAS (Semilla: %u) ===\n", semilla);
    generar_archivo_posteos(ARCHIVO_POSTEOS, CANT_POSTEOS);
    generar_lote_concentrado(ARCHIVO_LOTE_CONCENTRADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);
    generar_lote_disperso(ARCHIVO_LOTE_DISPERSO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);
    generar_lote_ordenado(ARCHIVO_LOTE_ORDENADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);

    const char *lotes[] = { ARCHIVO_LOTE_CONCENTRADO, ARCHIVO_LOTE_DISPERSO, ARCHIVO_LOTE_ORDENADO };
    const char *nombresLotes[] = { "Concentrado", "Disperso  ", "Ordenado   " };

    printf("\n=========================================================================================================\n");
    printf(" %-12s | %-16s | %-5s | %-12s | %-12s | %-14s | %-8s\n",
           "Lote", "Estructura", "Cap.", "T. Total (s)", "Prom/Post(ms)", "Hits (%)", "Alt. ABB");
    printf("=========================================================================================================\n");

    for(int l = 0; l < 3; l++)
    {
        /* Caso Sin Cache */
        procesar_lote_usuarios(lotes[l], ARCHIVO_POSTEOS, MODO_SIN_CACHE, 0, &m);
        printf(" %-12s | %-16s | %-5s | %-12.4f | %-12.6f | %-14s | %-8s\n",
               nombresLotes[l], "Sin Cache", "-", m.tiempoTotalSeg, m.tiempoPromPosteoMs, "-", "-");

        /* Variando capacidades para Parte 1 y Parte 2 */
        for(size_t c = 0; c < cantCapacidades; c++)
        {
            unsigned cap = capacidades[c];

            /* Parte 1 */
            procesar_lote_usuarios(lotes[l], ARCHIVO_POSTEOS, MODO_CACHE_PARTE_1, cap, &m);
            double pctP1 = m.totalConsultas > 0 ? ((double)m.aciertos * 100.0) / m.totalConsultas : 0.0;
            char hitsStrP1[32];
            snprintf(hitsStrP1, sizeof(hitsStrP1), "%u (%.1f%%)", m.aciertos, pctP1);

            printf(" %-12s | %-16s | %-5u | %-12.4f | %-12.6f | %-14s | %-8s\n",
                   nombresLotes[l], "Parte 1 (Lista)", cap, m.tiempoTotalSeg, m.tiempoPromPosteoMs, hitsStrP1, "-");

            /* Parte 2 */
            procesar_lote_usuarios(lotes[l], ARCHIVO_POSTEOS, MODO_CACHE_PARTE_2, cap, &m);
            double pctP2 = m.totalConsultas > 0 ? ((double)m.aciertos * 100.0) / m.totalConsultas : 0.0;
            char hitsStrP2[32];
            snprintf(hitsStrP2, sizeof(hitsStrP2), "%u (%.1f%%)", m.aciertos, pctP2);
            char altStr[16];
            snprintf(altStr, sizeof(altStr), "%u", m.alturaArbol);

            printf(" %-12s | %-16s | %-5u | %-12.4f | %-12.6f | %-14s | %-8s\n",
                   nombresLotes[l], "Parte 2 (ABB+LD)", cap, m.tiempoTotalSeg, m.tiempoPromPosteoMs, hitsStrP2, altStr);
        }
        printf("---------------------------------------------------------------------------------------------------------\n");
    }
    printf("=========================================================================================================\n\n");
}

void menu_interactivo(void)
{
    int opcion = -1;
    unsigned semilla = 12345;
    tMetricas m;

    while(opcion != 0)
    {
        printf("\n======================================================\n");
        printf("           SIMULADOR LRU CACHE - TWEENER              \n");
        printf("======================================================\n");
        printf(" 1. Generar archivo de posteos (%s, %d registros)\n", ARCHIVO_POSTEOS, CANT_POSTEOS);
        printf(" 2. Generar lote concentrado (%s)\n", ARCHIVO_LOTE_CONCENTRADO);
        printf(" 3. Generar lote disperso (%s)\n", ARCHIVO_LOTE_DISPERSO);
        printf(" 4. Generar lote ordenado (%s)\n", ARCHIVO_LOTE_ORDENADO);
        printf(" 5. Probar rendimiento de un lote\n");
        printf(" 6. Ejecutar bateria completa para informe (Tablas comparativas)\n");
        printf(" 0. Salir\n");
        printf("------------------------------------------------------\n");
        printf(" Seleccione una opcion: ");

        if(scanf("%d", &opcion) != 1)
        {
            while(getchar() != '\n'); // con esto vacío el buffer del stdin, hasta el enter.
            continue;
        }

        switch(opcion)
        {
            case 1:
                printf("\nGenerando archivo de posteos...\n");
                if(generar_archivo_posteos(ARCHIVO_POSTEOS, CANT_POSTEOS))
                    printf("-> Archivo \"%s\" generado correctamente.\n", ARCHIVO_POSTEOS);
                break;

            case 2:
                printf("Ingrese semilla numerica (ej. 12345): ");
                if(scanf("%u", &semilla) == 1)
                {
                    if(generar_lote_concentrado(ARCHIVO_LOTE_CONCENTRADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla))
                        printf("-> Lote concentrado \"%s\" generado con exito.\n", ARCHIVO_LOTE_CONCENTRADO);
                }
                break;

            case 3:
                printf("Ingrese semilla numerica (ej. 12345): ");
                if(scanf("%u", &semilla) == 1)
                {
                    if(generar_lote_disperso(ARCHIVO_LOTE_DISPERSO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla))
                        printf("-> Lote disperso \"%s\" generado con exito.\n", ARCHIVO_LOTE_DISPERSO);
                }
                break;

            case 4:
                printf("Ingrese semilla numerica (ej. 12345): ");
                if(scanf("%u", &semilla) == 1)
                {
                    if(generar_lote_ordenado(ARCHIVO_LOTE_ORDENADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla))
                        printf("-> Lote ordenado \"%s\" generado con exito (Semilla: %u).\n", ARCHIVO_LOTE_ORDENADO, semilla);
                }
                break;

            case 5:
            {
                int opcLote = 1, modo = 0;
                unsigned cap = 50;
                const char *rutaLote = ARCHIVO_LOTE_CONCENTRADO;

                printf("\nSeleccione Lote:\n");
                printf(" 1. Concentrado (%s)\n", ARCHIVO_LOTE_CONCENTRADO);
                printf(" 2. Disperso    (%s)\n", ARCHIVO_LOTE_DISPERSO);
                printf(" 3. Ordenado    (%s)\n", ARCHIVO_LOTE_ORDENADO);
                printf(" Opcion: ");
                if(scanf("%d", &opcLote) == 1)
                {
                    if(opcLote == 2) rutaLote = ARCHIVO_LOTE_DISPERSO;
                    else if(opcLote == 3) rutaLote = ARCHIVO_LOTE_ORDENADO;
                }

                printf("Seleccione Modo:\n");
                printf(" 0. Sin Cache (Directo a disco)\n");
                printf(" 1. LRU Cache Parte 1 (Lista Simple)\n");
                printf(" 2. LRU Cache Parte 2 (Lista Doble + Arbol Indice)\n");
                printf(" Opcion: ");
                if(scanf("%d", &modo) == 1)
                {
                    if(modo != 0)
                    {
                        printf("Ingrese capacidad de la cache: ");
                        if(scanf("%u", &cap) != 1) cap = 50;
                    }
                    if(procesar_lote_usuarios(rutaLote, ARCHIVO_POSTEOS, (tModoCache)modo, cap, &m))
                        mostrar_metricas(&m, rutaLote, (tModoCache)modo, cap);
                }
                break;
            }

            case 6:
                printf("Ingrese semilla para reproducir los lotes (ej. 12345): ");
                if(scanf("%u", &semilla) == 1)
                    ejecutar_bateria_informe(semilla);
                break;

            case 0:
                printf("\nSaliendo del programa...\n");
                break;

            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        int comando = atoi(argv[1]);
        tMetricas m;

        switch(comando)
        {
            case 1:
                generar_archivo_posteos(ARCHIVO_POSTEOS, CANT_POSTEOS);
                printf("Archivo de posteos generado: %s\n", ARCHIVO_POSTEOS);
                break;

            case 2:
            {
                unsigned semilla = (argc > 2) ? (unsigned)atoi(argv[2]) : 12345;
                generar_lote_concentrado(ARCHIVO_LOTE_CONCENTRADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);
                printf("Lote concentrado generado: %s (Semilla: %u)\n", ARCHIVO_LOTE_CONCENTRADO, semilla);
                break;
            }

            case 3:
            {
                unsigned semilla = (argc > 2) ? (unsigned)atoi(argv[2]) : 12345;
                generar_lote_disperso(ARCHIVO_LOTE_DISPERSO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);
                printf("Lote disperso generado: %s (Semilla: %u)\n", ARCHIVO_LOTE_DISPERSO, semilla);
                break;
            }

            case 4:
            {
                unsigned semilla = (argc > 2) ? (unsigned)atoi(argv[2]) : 12345;
                generar_lote_ordenado(ARCHIVO_LOTE_ORDENADO, CANT_USUARIOS_DEFAULT, CANT_POSTEOS, semilla);
                printf("Lote ordenado generado: %s (Semilla: %u)\n", ARCHIVO_LOTE_ORDENADO, semilla);
                break;
            }

            case 5:
            {
                if(argc < 4)
                {
                    imprimir_ayuda(argv[0]);
                    return 1;
                }
                const char *rutaLote = argv[2];
                tModoCache modo = (tModoCache)atoi(argv[3]);
                unsigned cap = (argc > 4) ? (unsigned)atoi(argv[4]) : 50;

                if(procesar_lote_usuarios(rutaLote, ARCHIVO_POSTEOS, modo, cap, &m))
                    mostrar_metricas(&m, rutaLote, modo, cap);
                else
                    fprintf(stderr, "Error procesando lote \"%s\". Verifique que existan los archivos.\n", rutaLote);
                break;
            }

            case 6:
            {
                unsigned semilla = (argc > 2) ? (unsigned)atoi(argv[2]) : 12345;
                ejecutar_bateria_informe(semilla);
                break;
            }

            default:
                imprimir_ayuda(argv[0]);
                break;
        }
        return 0;
    }

    menu_interactivo();
    return 0;
}
