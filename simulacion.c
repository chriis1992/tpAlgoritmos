#include "simulacion.h"

int comparar_posteos_id(const void *a, const void *b)
{
    const tPosteo *p1 = (const tPosteo *)a;
    const tPosteo *p2 = (const tPosteo *)b;

    if(p1->id < p2->id)
        return -1;

    if(p1->id > p2->id)
        return 1;

    return 0;
}
