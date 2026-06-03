#ifndef BALL_H
#define BALL_H

#include <stdint.h>
#include "player.h"
#include "utils.h"

typedef struct AppState AppState;

#define INIT_BALL_SPEED 10
#define BALL_SPEEP_INC 5
#define MAX_BALL_SPEED 35
#define BALL_SIZE 10

typedef struct Ball {
    VectorInt pos;
    VectorInt vel;
    uint32_t speed;
} __attribute__((packed)) Ball;

void update_ball(AppState* app_state);
void draw_ball(Ball* ball);

void handle_wall_ball(AppState* app_state);
void handle_player_ball(Ball* ball, Player* player);

int32_t generate_random_vel(int lower_bound, int upper_bound);

#endif
