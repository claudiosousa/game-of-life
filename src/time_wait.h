/**
 * Some helper functions for managing execution time.
 * @author David Gonzalez, Claudio Sousa
 */

#ifndef TIME_WAIT_H
#define TIME_WAIT_H

#include <time.h>

void time_wait_start(struct timespec *tm);
void time_wait_freq(struct timespec *start_time, int frequency);

#endif
