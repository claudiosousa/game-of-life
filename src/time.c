/**
 * Some helper functions for managing execution time.
 * @author David Gonzalez, Claudio Sousa
 */

#include <stdio.h>
#include <unistd.h>
#include "time.h"

static const double NANOSECONDS_IN_SECOND = 1000000000;
static const double MICROSECONDS_IN_SECOND = 1000000;

void time_start(struct timespec *tm) {
    if (clock_gettime(CLOCK_MONOTONIC, tm) != 0)
        perror("Time start failed");
}

void time_wait_freq(struct timespec *start_time, int frequency)
{
    int wait_time = MICROSECONDS_IN_SECOND / frequency;
    struct timespec end_time;

    if (start_time != NULL)
    {
        if (clock_gettime(CLOCK_MONOTONIC, &end_time) == 0)
        {
            wait_time -= (end_time.tv_sec - start_time->tv_sec) * MICROSECONDS_IN_SECOND;
            wait_time -= (end_time.tv_nsec - start_time->tv_nsec) / 1000;
        }
        else
        {
            perror("Time stop failed");
        }
    }

    usleep(wait_time);
}
