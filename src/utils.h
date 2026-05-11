#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define FLOAT_SCALE 1000

typedef struct VectorInt {
    int32_t x;
    int32_t y;
} __attribute__((packed)) VectorInt;

void die(const char* s);
void print_log(const char* s, ...);

#endif
