#ifndef TIME_MEASURE_H
#define TIME_MEASURE_H

#include <stdio.h>
#include <time.h>

#define MEASURE_TIME_INIT() \
    struct timespec start_time, end_time; \
    double elapsed_time = 0.0;

#define MEASURE_TIME(run_code) \
    clock_gettime(CLOCK_REALTIME, &start_time); \
    run_code; \
    clock_gettime(CLOCK_REALTIME, &end_time); \
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000.0 + \
                   (end_time.tv_nsec - start_time.tv_nsec) / 1000.0; \
    printf(" %f us |\r\n", elapsed_time);

#endif /* TIME_MEASURE_H */
