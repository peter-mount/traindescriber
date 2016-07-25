
/* 
 * File:   stats.h
 * Author: peter
 *
 * Created on 22 July 2016, 07:54
 */

#ifndef STATS_H
#define STATS_H

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct statistic {
        // The current/latest value
        long current;
        // the min value
        long min;
        // the max value
        long max;
        // the total
        long total;
        // number of samples
        long count;
        // Mutex
        pthread_mutex_t mutex;
    };
    
    struct iot_statistic {
        struct statistic stat;
        // The period in seconds between reset times
        int period;
        // The IoT channel key
        char *key;
    };

    extern void statistic_init(struct statistic *s);
    extern void statistic_set( struct statistic *s, long value );
    extern void statistic_reset( struct statistic *s, struct statistic *d);
    

#ifdef __cplusplus
}
#endif

#endif /* STATS_H */

