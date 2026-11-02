#include "simulacion.h"

/*
 =========================================================================
 Prototipos de funciones auxiliares del modulo
 =========================================================================
*/

int contiene_post(const int feed[], size_t usados, int id);
void mezclar(int v[], size_t n);
size_t agregar_posts_bloque(int feed[], size_t *usados, size_t inicio, size_t cantidad, size_t k);
void agregar_posts_aleatorios(int feed[], size_t *usados, unsigned cantPosteosTotal, size_t cantidadRequerida);
static unsigned aleatorio_hasta(unsigned limite);

/*
 =========================================================================
 Implementacion de funciones auxiliares
 =========================================================================
*/

/* Combino dos RAND_MAX ya que capaz que llega a 32767 nomás. */
static unsigned aleatorio_hasta(unsigned limite)
{
    unsigned long long base = (unsigned long long)RAND_MAX + 1;
    unsigned long long valor = (unsigned long long)rand() * base + (unsigned long long)rand();
    return (unsigned)(valor % limite);
}

int contiene_post(const int feed[], size_t usados, int id)
{
    for(size_t i = 0; i < usados; i++)
    {
        if(feed[i] == id)
            return 1;
    }
    return 0;
}

void mezclar(int v[], size_t n)
{
    for(size_t i = n; i > 1; i--)
    {
        size_t j = aleatorio_hasta((unsigned)i);
        int aux = v[i - 1];
        v[i - 1] = v[j];
        v[j] = aux;
    }
}

size_t agregar_posts_bloque(int feed[], size_t *usados, size_t inicio, size_t cantidad, size_t k)
{
    int *candidatos;
    size_t n = 0;

    if(k == 0 || cantidad == 0)
        return 0;

    candidatos = malloc(cantidad * sizeof(*candidatos));
    if(!candidatos)
        return 0;

    for(size_t i = 0; i < cantidad; i++)
    {
        int id = (int)(inicio + i);
        if(!contiene_post(feed, *usados, id))
            candidatos[n++] = id;
    }

    mezclar(candidatos, n);

    size_t agregar = (n < k) ? n : k;
    for(size_t i = 0; i < agregar; i++)
        feed[(*usados)++] = candidatos[i];

    free(candidatos);
    return agregar;
}

void agregar_posts_aleatorios(int feed[], size_t *usados, unsigned cantPosteosTotal, size_t cantidadRequerida)
{
    if(!feed || !usados || cantPosteosTotal == 0)
        return;

    // El limite no puede ser superior ni a la cantidad requerida ni al total de posteos disponibles en el universo.
    // Esto evita cualquier bucle infinito cuando los posteos disponibles son menores que la meta a completar.
    size_t limite = (cantidadRequerida < (size_t)cantPosteosTotal) ? cantidadRequerida : (size_t)cantPosteosTotal;

    while(*usados < limite)
    {
        int idAleatorio = (int)aleatorio_hasta(cantPosteosTotal);
        if(!contiene_post(feed, *usados, idAleatorio))
            feed[(*usados)++] = idAleatorio;
    }
}

/*
 =========================================================================
 Manejo de Archivos y Metricas
 =========================================================================
*/

int abrirArchivo(FILE **fp, const char *nombreArchivo, const char *modoApertura, int mostrarError)
{
    if(!fp || !nombreArchivo || !modoApertura)
        return 0;

    *fp = fopen(nombreArchivo, modoApertura);
    if(*fp == NULL)
    {
        if(mostrarError == 1)
            fprintf(stderr, "Error abriendo \"%s\" en modo \"%s\".\n", nombreArchivo, modoApertura);
        return 0;
    }
    return 1;
}

void inicializar_metricas(tMetricas *metrica)
{
    if(!metrica)
        return;

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
    return (p1->id > p2->id) - (p1->id < p2->id);
}

/*
 =========================================================================
 Generacion de Archivos de Posteos y Lotes
 =========================================================================
*/

int generar_archivo_posteos(const char *rutaPosteos, unsigned cantPosteos)
{
    FILE *pf;
    unsigned i;
    tPosteo post;

    if(!rutaPosteos || cantPosteos == 0)
        return 0;

    if(!abrirArchivo(&pf, rutaPosteos, "wb", 1))
        return 0;

    for(i = 0; i < cantPosteos; i++)
    {
        post.id = (int)i;
        snprintf(post.texto, sizeof(post.texto), "Posteo %u prueba de Tweener y LRU Cache.", i);
        if(fwrite(&post, sizeof(tPosteo), 1, pf) != 1)
        {
            fclose(pf);
            return 0;
        }
    }

    if(fclose(pf) != 0)
        return 0;

    return 1;
}

int leer_posteo_disco(FILE *pfPosteos, int id, tPosteo *post, tMetricas *metrica)
{
    if(!pfPosteos || !post || id < 0)
        return 0;

    if(fseek(pfPosteos, (long)id * (long)sizeof(tPosteo), SEEK_SET) != 0)
        return 0;

    if(fread(post, sizeof(tPosteo), 1, pfPosteos) != 1)
        return 0;

    if(metrica)
        metrica->lecturasDisco++;

    return 1;
}

int generar_lote_concentrado(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla)
{
    FILE *pf;
    unsigned u;

    if(!rutaLote || cantUsuarios == 0 || cantPosteosTotal < TAM_FEED)
        return 0;

    if(!abrirArchivo(&pf, rutaLote, "wb", 1))
        return 0;

    srand(semilla);

    /*
       Ventana de posteos populares:
       1) 1/3 del feed (6 posts) son IDENTICOS para TODOS los usuarios.
       2) Un segundo tercio proviene del bloque de populares (tamBloque = 40 posts).
       3) El tercio restante se elige aleatoriamente del universo de 50.000 posts.
    */
    unsigned tamBloque = TAM_FEED * 2;
    if(tamBloque > cantPosteosTotal)
        tamBloque = cantPosteosTotal;

    unsigned iniBloque = (cantPosteosTotal > tamBloque) ? aleatorio_hasta(cantPosteosTotal - tamBloque + 1) : 0;
    size_t cantComunes = TAM_FEED / 3;
    size_t cantPopulares = (TAM_FEED - cantComunes) / 2;

    for(u = 0; u < cantUsuarios; u++)
    {
        tUsuario user;
        size_t usados = 0;
        sprintf(user.usuario, "user%u", u);

        // 1/3. Posteos comunes a todos
        for(size_t c = 0; c < cantComunes; c++)
            user.feed[usados++] = (int)(iniBloque + c);

        // 2/3. Posteos del bloque de populares
        if(agregar_posts_bloque(user.feed, &usados, iniBloque, tamBloque, cantPopulares) != cantPopulares)
        {
            fclose(pf);
            return 0;
        }

        // 3/3. Posteos aleatorios del universo total
        agregar_posts_aleatorios(user.feed, &usados, cantPosteosTotal, TAM_FEED);

        if(usados != TAM_FEED)
        {
            fclose(pf);
            return 0;
        }

        if(fwrite(&user, sizeof(tUsuario), 1, pf) != 1)
        {
            fclose(pf);
            return 0;
        }
    }

    if(fclose(pf) != 0)
        return 0;

    return 1;
}

int generar_lote_disperso(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla)
{
    FILE *pf;
    unsigned u;

    if(!rutaLote || cantUsuarios == 0 || cantPosteosTotal < TAM_FEED)
        return 0;

    if(!abrirArchivo(&pf, rutaLote, "wb", 1))
        return 0;

    srand(semilla);

    /*
      En el lote disperso, cada usuario tiene posteos tomados aleatoriamente de todo el espectro de 50.000 posteos, sin ningun bloque en comun.
      La probabilidad de que un usuario comparta IDs con otro usuario en la ventana de la cache es minima.
    */
    for(u = 0; u < cantUsuarios; u++)
    {
        tUsuario user;
        size_t usados = 0;
        sprintf(user.usuario, "user%u", u);

        agregar_posts_aleatorios(user.feed, &usados, cantPosteosTotal, TAM_FEED);

        if(usados != TAM_FEED)
        {
            fclose(pf);
            return 0;
        }

        if(fwrite(&user, sizeof(tUsuario), 1, pf) != 1)
        {
            fclose(pf);
            return 0;
        }
    }

    if(fclose(pf) != 0)
        return 0;

    return 1;
}

int generar_lote_ordenado(const char *rutaLote, unsigned cantUsuarios, unsigned cantPosteosTotal, unsigned semilla)
{
    FILE *pf;
    unsigned u;

    if(!rutaLote || cantUsuarios == 0 || cantPosteosTotal < TAM_FEED)
        return 0;

    if(!abrirArchivo(&pf, rutaLote, "wb", 1))
        return 0;

    srand(semilla);

    /*
      En el lote ordenado, los feeds contienen IDs estrictamente crecientes.
      La semilla determina el punto de partida inicial y los saltos variables.
      Al ser todas las consultas crecientes (x_1 < x_2 < ... < x_n), en el ABB cada clave se inserta siempre a la derecha (arbol->der),
      degenerandolo en una lista enlazada de altura maxima h = capacidad de la cache.
    */
    unsigned totalConsultas = cantUsuarios * TAM_FEED;
    unsigned margenSalto = totalConsultas * 2;
    unsigned idActual;

    if(cantPosteosTotal > margenSalto)
        idActual = aleatorio_hasta(cantPosteosTotal - margenSalto);
    else if(cantPosteosTotal > totalConsultas)
        idActual = aleatorio_hasta(cantPosteosTotal - totalConsultas);
    else
        idActual = 0;

    for(u = 0; u < cantUsuarios; u++)
    {
        tUsuario user;
        sprintf(user.usuario, "user%u", u);

        for(int p = 0; p < TAM_FEED; p++)
        {
            user.feed[p] = (int)idActual;

            // Avance estrictamente creciente: salto de 1 o 2 unidades
            if(idActual + 2 < cantPosteosTotal)
                idActual += 1 + aleatorio_hasta(2);
            else
                idActual = (idActual + 1) % cantPosteosTotal;
        }

        if(fwrite(&user, sizeof(tUsuario), 1, pf) != 1)
        {
            fclose(pf);
            return 0;
        }
    }

    if(fclose(pf) != 0)
        return 0;

    return 1;
}

/*
 =========================================================================
 Simulador de Lecturas y Medicion de Tiempos
 =========================================================================
*/

int procesar_lote_usuarios(const char *rutaLote, const char *rutaPosteos, tModoCache modo, unsigned capacidadCache, tMetricas *metricas)
{
    FILE *pfLote = NULL;
    FILE *pfPosteos = NULL;
    tUsuario usuario;
    t_lru_cache cacheP1;
    t_lru_cache_lista_doble cacheP2;
    clock_t tInicio, tFin;
    int correcto = 1;

    if(!rutaLote || !rutaPosteos || !metricas)
        return 0;

    if(modo != MODO_SIN_CACHE && modo != MODO_CACHE_PARTE_1 && modo != MODO_CACHE_PARTE_2)
        return 0;

    if(modo != MODO_SIN_CACHE && capacidadCache == 0)
        return 0;

    if(!abrirArchivo(&pfLote, rutaLote, "rb", 1))
        return 0;

    if(!abrirArchivo(&pfPosteos, rutaPosteos, "rb", 1))
    {
        fclose(pfLote);
        return 0;
    }

    inicializar_metricas(metricas);

    if(modo == MODO_CACHE_PARTE_1)
        crear_lrucache(&cacheP1, capacidadCache, comparar_posteos_id);
    else if(modo == MODO_CACHE_PARTE_2)
        crear_lrucache_lista_doble(&cacheP2, capacidadCache, comparar_posteos_id);

    tInicio = clock();

    while(correcto && fread(&usuario, sizeof(tUsuario), 1, pfLote) == 1)
    {
        metricas->cantUsuarios++;

        for(int j = 0; j < TAM_FEED && correcto; j++)
        {
            int idPost = usuario.feed[j];
            tPosteo post;
            post.id = idPost;
            metricas->totalConsultas++;

            if(modo == MODO_SIN_CACHE)
            {
                if(!leer_posteo_disco(pfPosteos, idPost, &post, metricas))
                    correcto = 0;
            }
            else if(modo == MODO_CACHE_PARTE_1)
            {
                if(obtener_lrucache(&cacheP1, &post, sizeof(tPosteo)))
                {
                    metricas->aciertos++;
                }
                else
                {
                    metricas->fallos++;
                    if(!leer_posteo_disco(pfPosteos, idPost, &post, metricas) ||
                       !agregar_lrucache(&cacheP1, &post, sizeof(tPosteo)))
                        correcto = 0;
                }
            }
            else if(modo == MODO_CACHE_PARTE_2)
            {
                if(obtener_lrucache_lista_doble(&cacheP2, &post, sizeof(tPosteo)))
                {
                    metricas->aciertos++;
                }
                else
                {
                    metricas->fallos++;
                    if(!leer_posteo_disco(pfPosteos, idPost, &post, metricas) ||
                       !agregar_lrucache_lista_doble(&cacheP2, &post, sizeof(tPosteo)))
                        correcto = 0;
                }
            }
        }
    }

    tFin = clock();

    if(ferror(pfLote) || tInicio == (clock_t)-1 || tFin == (clock_t)-1)
        correcto = 0;

    if(correcto)
    {
        metricas->tiempoTotalSeg = (double)(tFin - tInicio) / CLOCKS_PER_SEC;

        if(metricas->cantUsuarios > 0)
            metricas->tiempoPromUsuarioMs = (metricas->tiempoTotalSeg * 1000.0) / metricas->cantUsuarios;

        if(metricas->totalConsultas > 0)
            metricas->tiempoPromPosteoMs = (metricas->tiempoTotalSeg * 1000.0) / metricas->totalConsultas;
    }

    if(modo == MODO_CACHE_PARTE_1)
    {
        vaciar_lrucache(&cacheP1);
    }
    else if(modo == MODO_CACHE_PARTE_2)
    {
        if(correcto)
            metricas->alturaArbol = altura_lrucache_lista_doble(&cacheP2);
        vaciar_lrucache_lista_doble(&cacheP2);
    }

    fclose(pfLote);
    fclose(pfPosteos);

    return correcto;
}

void mostrar_metricas(const tMetricas *metricas, const char *nombreLote, tModoCache modo, unsigned capacidad)
{
    const char *modoStr = "Sin Cache";
    if(modo == MODO_CACHE_PARTE_1)
        modoStr = "LRU Cache Parte 1 (Lista Simple)";
    else if(modo == MODO_CACHE_PARTE_2)
        modoStr = "LRU Cache Parte 2 (Lista Doble + ABB)";

    printf("\n============================================================\n");
    printf(" Lote: %s\n", nombreLote);
    printf(" Modo: %s", modoStr);
    if(modo != MODO_SIN_CACHE)
        printf(" (Capacidad: %u)", capacidad);
    printf("\n============================================================\n");
    printf(" Usuarios procesados:    %u\n", metricas->cantUsuarios);
    printf(" Consultas a posteos:    %u\n", metricas->totalConsultas);
    printf(" Lecturas a disco:       %u\n", metricas->lecturasDisco);
    if(modo != MODO_SIN_CACHE)
    {
        double tasaAciertos = (metricas->totalConsultas > 0) ?
            ((double)metricas->aciertos * 100.0) / metricas->totalConsultas : 0.0;
        printf(" Aciertos en cache:      %u (%.2f%%)\n", metricas->aciertos, tasaAciertos);
        printf(" Fallos en cache:        %u (%.2f%%)\n", metricas->fallos, 100.0 - tasaAciertos);
        if(modo == MODO_CACHE_PARTE_2)
            printf(" Altura del arbol:       %u\n", metricas->alturaArbol);
    }
    printf(" Tiempo total:           %.4f s\n", metricas->tiempoTotalSeg);
    printf(" Promedio por usuario:   %.4f ms\n", metricas->tiempoPromUsuarioMs);
    printf(" Promedio por posteo:    %.6f ms\n", metricas->tiempoPromPosteoMs);
    printf("============================================================\n\n");
}
