/**
 * Some helper functions for managing execution time.
 * @author David Gonzalez, Claudio Sousa
 */

#ifndef TIME_H
#define TIME_H

#include <time.h>

/**
 * Starts a time measure.
 * @param tm Structure to hold the starting time.
 */
void time_start(struct timespec *tm);

/**
 * Make the calling thread wait the time remaining
 * @param start_time Starting time
 * @param frequency Execution freqeuncy in Hz
 */
void time_wait_freq(struct timespec *start_time, int frequency);

#endif
