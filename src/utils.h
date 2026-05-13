#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define FLOAT_SCALE 1000

#define WIN_WIDTH 100
#define WIN_HEIGHT 30

#define BALL_COLOR_PAIR 1
#define BG_COLOR_PAIR 2

typedef struct VectorInt {
    int32_t x;
    int32_t y;
} __attribute__((packed)) VectorInt;

void die(const char* s);
void print_log(const char* s, ...);

#endif
