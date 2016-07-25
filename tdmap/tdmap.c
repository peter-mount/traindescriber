
/* 
 * File:   corpus-import.c
 * Author: peter
 *
 * Created on 17 July 2016, 21:20
 */

#include <stdio.h>
#include <stdlib.h>
#include <area51/json.h>
#include <area51/log.h>
#include <area51/rest.h>
#include <area51/string.h>
#include <networkrail/stats.h>
#include <networkrail/traindescriber.h>

#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdbool.h>

struct TDMap map;

static int about() {
    logconsole("Usage: tdmap map file...\n");
    return EXIT_FAILURE;
}

static bool forEachArea(void *key, void *value, void *context) {
    struct Area *area = (struct Area *) value;
    logconsole("Area: %s %s Berths: %d", key, area->area, hashmapSize(area->berths));

    char *fname = genurl((const char *) context, area->area);

    FILE *f = fopen(fname, "w");
    if (f) {
        td_writeArea(area, f);
        fclose(f);
    }
    free(fname);

    return true;
}

static void importMessage(struct json_object * obj) {
    struct array_list *list = json_object_get_array(obj);
    int len = array_list_length(list);
    for (int i = 0; i < len; i++) {
        json_object *msg = (json_object *) array_list_get_idx(list, i);

        json_object *m;
        if (json_object_object_get_ex(msg, "CA_MSG", &m)) {
            td_berthStep(&map, json_getString(m, "area_id"), json_getString(m, "from"), json_getString(m, "to"), json_getString(m, "descr"));
        } else if (json_object_object_get_ex(msg, "CB_MSG", &m)) {
            td_berthCancel(&map, json_getString(m, "area_id"), json_getString(m, "from"));
        } else if (json_object_object_get_ex(msg, "CC_MSG", &m)) {
            td_berthInterpose(&map, json_getString(m, "area_id"), json_getString(m, "to"), json_getString(m, "descr"));
        }
    }
}

static void import(const char *fmap, const long size) {
    char *s = (char *) fmap;
    char *p = s;
    char *e = &s[size];
    while (p < e) {
        if (*p == '\n') {
            int l = p - s;
            char *m = (char *) malloc(l + 1);
            if (m) {
                memcpy(m, s, l);
                m[l] = 0;

                struct json_object *obj = json_tokener_parse(m);
                if (obj) {
                    importMessage(obj);
                    json_object_put(obj);
                }

                free(m);
            }

            s = ++p;
        } else p++;
    }
}

int importTdFile(char *fname) {
    logconsole("Reading %s", fname);
    int fsock = open(fname, O_RDONLY);
    if (fsock == -1) {
        logconsole("Failed to open %s", fname);
        return EXIT_FAILURE;
    }

    struct stat sb;

    if (fstat(fsock, &sb) == -1) {
        close(fsock);
        return EXIT_FAILURE;
    }

    void *fmap = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fsock, 0);
    if (fmap == MAP_FAILED) {
        close(fsock);
        return EXIT_FAILURE;
    }

    import(fmap, sb.st_size);

    munmap(fmap, sb.st_size);
    close(fsock);
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    if (argc < 3)
        return about();

    td_init(&map);

    int rc = EXIT_SUCCESS;
    for (int i = 2; i < argc && rc == EXIT_SUCCESS; i++)
        rc = importTdFile(argv[i]);

    hashmapForEach(map.areas, forEachArea, argv[1]);

    return rc;
}

