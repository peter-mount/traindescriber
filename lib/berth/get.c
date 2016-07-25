#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/traindescriber.h>

struct Berth *td_getBerth(struct Area *area, char *berth) {
    if(!area || !berth)return NULL;
    if (0 != pthread_mutex_lock(&area->mutex)) return NULL;

    struct Berth *b = (struct Berth *) hashmapGet(area->berths, berth);
    if (!b) {
        b = (struct Berth *) malloc(sizeof (struct Berth));
        memset(b, 0, sizeof (struct Berth));
        strncpy(b->name, berth, TD_BERTH_LEN);
        hashmapPut(area->berths, b->name, b);
    }

    pthread_mutex_unlock(&area->mutex);
    return b;
}
