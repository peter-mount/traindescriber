#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/stats.h>

void statistic_reset(struct statistic *s, struct statistic *d) {
    if (0 == pthread_mutex_lock(&s->mutex)) {
        if (d)
            memcpy(d, s, sizeof (struct statistic));

        s->current = s->total = s->count = 0;
        s->min = INT_MAX;
        s->max = INT_MIN;
        pthread_mutex_unlock(&s->mutex);
    }
}
