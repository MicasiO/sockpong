#ifndef BALL_H
#define BALL_H

#include "utils.h"

typedef struct Ball {
    Vector pos;
    Vector vel;
} __attribute__((packed)) Ball;

#endif
