#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/traindescriber.h>

struct Area *td_getArea(struct TDMap *map, char *area) {
    if (!area)return NULL;
    if (0 != pthread_mutex_lock(&map->mutex)) return NULL;

    struct Area *a = (struct Area *) hashmapGet(map->areas, area);
    if (!a) {
        a = (struct Area *) malloc(sizeof (struct Area));
        memset(a, 0, sizeof (struct Area));
        strncpy(a->area, area, TD_AREA_LEN);
        pthread_mutex_init(&a->mutex, NULL);
        a->berths = hashmapCreate(10, hashmapStringHash, hashmapStringEquals);

        if (a->berths)
            hashmapPut(map->areas, a->area, a);
        else {
            free(a);
            a = NULL;
        }
    }

    pthread_mutex_unlock(&map->mutex);
    return a;
}
