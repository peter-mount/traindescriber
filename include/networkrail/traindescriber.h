
/* 
 * File:   traindescriber.h
 * Author: peter
 *
 * Created on 22 July 2016, 07:48
 */

#ifndef TRAINDESCRIBER_H
#define TRAINDESCRIBER_H

#include <pthread.h>
#include <area51/hashmap.h>
#include <area51/list.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TD_AREA_LEN 2
#define TD_BERTH_LEN 4
#define TD_DESC_LEN 4

    struct Area {
        // Area is 2 char label
        char area[TD_AREA_LEN + 1];
        pthread_mutex_t mutex;
        Hashmap *berths;
    };

    struct Berth {
        // Name is 4 char label
        char name[TD_BERTH_LEN + 1];
        // Occupier
        char val[TD_DESC_LEN + 1];
        short nextcount;
        char **next;
    };

    struct TDMap {
        pthread_mutex_t mutex;

        // Map of TD areas. Each area is a map of berths
        Hashmap *areas;
    };

    extern void td_init(struct TDMap *map);
    extern struct Area *td_getArea(struct TDMap *map, char *area);
    extern struct Berth *td_getBerth(struct Area *area, char *berth);
    
    extern int td_writeArea(struct Area *area, FILE *f);
    
    extern void td_berthCancel(struct TDMap *map, char *area, char *berth);
    extern void td_berthInterpose(struct TDMap *map, char *area, char *berth, char *desc);
    extern void td_berthStep(struct TDMap *map, char *area, char *from, char *to, char *desc);
    
#ifdef __cplusplus
}
#endif

#endif /* TRAINDESCRIBER_H */

