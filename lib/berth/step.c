#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <area51/log.h>
#include <networkrail/traindescriber.h>

static void link(struct Area *a, struct Berth *f, struct Berth *t) {
    //logconsole("Linking %s %s -> %s %d", a->area, f->name, t->name, f->nextcount + 1);
    short oldLen = f->nextcount;
    char **oldAry = f->next;
    f->nextcount++;
    f->next = (char **) malloc(sizeof (char *)*f->nextcount);
    memcpy(f->next, oldAry, sizeof (char *)*oldLen);
    f->next[oldLen] = t->name;
}

void td_berthStep(struct TDMap *map, char *area, char *from, char *to, char *desc) {
    if (area && from && to) {
        struct Area *a = td_getArea(map, area);
        struct Berth *f = td_getBerth(a, from);
        struct Berth *t = td_getBerth(a, to);
        if (a && f && t && 0 == pthread_mutex_lock(&a->mutex)) {

            // Move the train
            f->val[0] = 0;
            if (desc)
                strncpy(t->val, desc, TD_DESC_LEN);
            else
                t->val[0] = 0;

            // Check next value to dynamically generate the map
            if (!f->nextcount)
                link(a, f, t);
            else {
                int i = 0;
                for (; i < f->nextcount; i++)
                    if (strcmp(f->next[i], t->name) == 0)
                        break;
                if (i == f->nextcount)
                    link(a, f, t);
            }
            pthread_mutex_unlock(&a->mutex);
        }
    }
}
