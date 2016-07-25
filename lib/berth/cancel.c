#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/traindescriber.h>

void td_berthCancel(struct TDMap *map, char *area, char *berth) {
    if (area && berth) {
        struct Area *a = td_getArea(map, area);
        struct Berth *b = td_getBerth(a, berth);
        if (a && b && 0 == pthread_mutex_lock(&a->mutex)) {
            b->val[0] = 0;
            pthread_mutex_unlock(&a->mutex);
        }
    }
}
