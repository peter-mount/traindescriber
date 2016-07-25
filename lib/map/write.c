#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdio.h>
#include <area51/log.h>
#include <networkrail/traindescriber.h>

static bool forEachBerth(void *key, void *value, void *context) {
    FILE *file = (FILE *) context;
    struct Berth *berth = (struct Berth *) value;
    fwrite(berth->name, TD_BERTH_LEN + 1, 1, file);
    fwrite(&berth->nextcount, sizeof (berth->nextcount), 1, file);
    for (int i = 0; i < berth->nextcount; i++)
        fwrite(berth->next[i], TD_BERTH_LEN + 1, 1, file);
    return true;
}

int td_writeArea(struct Area *area, FILE *file) {
    if (!area)
        return EXIT_FAILURE;

    if (0 != pthread_mutex_lock(&area->mutex))
        return EXIT_FAILURE;

    logconsole("Writing area %s", area->area);
    
    fwrite(area->area, TD_AREA_LEN + 1, 1, file);

    int berths = hashmapSize(area->berths);
    fwrite(&berths,sizeof(berths),1,file);
    hashmapForEach(area->berths, forEachBerth, file);

    pthread_mutex_unlock(&area->mutex);
    return EXIT_SUCCESS;
}
