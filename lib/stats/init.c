#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <networkrail/stats.h>

void statistic_init(struct statistic *s) {
    memset(s, 0, sizeof (struct statistic));
    pthread_mutex_init(&s->mutex, NULL);
    statistic_reset(s, NULL);
}
