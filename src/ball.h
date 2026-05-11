#ifndef BALL_H
#define BALL_H

#include <ncurses.h>
#include <stdint.h>
#include "player.h"
#include "utils.h"

#define BALL_SPEED 1
#define BALL_SIZE 1

#define BALL_COLOR_PAIR 1

typedef struct Ball {
    VectorInt pos;
    VectorInt vel;
} __attribute__((packed)) Ball;

void update_ball(Ball* ball, Player* player1, Player* player2, VectorInt win_size);
void draw_ball(Ball* ball, WINDOW* win);

void handle_wall_ball(Ball* ball, VectorInt win_size);
void handle_player_ball(Ball* ball, Player* player);

int32_t generate_random_vel(int lower_bound, int upper_bound);

#endif
