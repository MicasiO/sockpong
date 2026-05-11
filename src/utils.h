#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#define PLAYER_SPEED 1
#define PLAYER_SIZE 10
#define BALL_SIZE 5

typedef struct Vector {
    uint32_t x;
    uint32_t y;
} __attribute__((packed)) Vector;

void die(const char* s);
void print_log(const char* s, ...);

#endif
