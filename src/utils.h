#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define FLOAT_SCALE 1000

#define WIN_WIDTH 800
#define WIN_HEIGHT 500

typedef enum colors { BALL_COLOR_PAIR = 1, BG_COLOR_PAIR, SCORE_COLOR_PAIR, COLOR_GRAY } colors;

typedef struct VectorInt {
    int32_t x;
    int32_t y;
} __attribute__((packed)) VectorInt;

typedef struct {
    struct timespec length;
    struct timespec start_time;
    bool done;
} Timer;

void die(const char* s);
void print_log(const char* s, ...);

void start_timer(Timer* timer);
double get_timer(Timer* timer);

#endif
