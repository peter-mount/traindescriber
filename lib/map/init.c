#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/traindescriber.h>

void td_init( struct TDMap *map ) {
    memset(map,0,sizeof(struct TDMap));
    pthread_mutex_init(&map->mutex, NULL);
    map->areas = hashmapCreate(10,hashmapStringHash,hashmapStringEquals);
}
