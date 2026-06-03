#include "utils.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void die(const char* s) {
    perror(s);
    exit(1);
}

void print_log(const char* s, ...) {
    FILE* file = fopen("logs.txt", "a+");
    if (!file) {
        die("file");
    }

    va_list args;
    va_start(args, s);

    vfprintf(file, s, args);

    fflush(file);
    fclose(file);

    va_end(args);
}

void start_timer(Timer* timer) {
    timer->done = false;
    clock_gettime(CLOCK_MONOTONIC, &timer->start_time);
}

double get_timer(Timer* timer) {
    struct timespec current_time;

    if (!timer->done) {
        clock_gettime(CLOCK_MONOTONIC, &current_time);
    } else {
        return 0.0;
    }

    double start_sec = timer->start_time.tv_sec + (timer->start_time.tv_nsec / 1e9);
    double current_sec = current_time.tv_sec + (current_time.tv_nsec / 1e9);
    double timer_length = timer->length.tv_sec + (timer->length.tv_nsec / 1e9);

    double remaining_sec = timer_length - (current_sec - start_sec);

    if (remaining_sec <= 0) {
        timer->done = true;
    }

    return remaining_sec;
}
