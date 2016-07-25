#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/stats.h>

void statistic_set(struct statistic *s, long value) {
    if (0 == pthread_mutex_lock(&s->mutex)) {

        s->current = value;
        s->total += value;
        if (value < s->min) s->min = value;
        if (value > s->max) s->max = value;
        s->count++;
        pthread_mutex_unlock(&s->mutex);
    }
}
