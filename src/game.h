#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <stdint.h>
#include "ball.h"
#include "player.h"

#define MAX_SCORE 5

typedef struct AppState AppState;

typedef struct GameState {
    Player player1;
    Player player2;
    Ball ball;
} __attribute__((packed)) GameState;

void loop(AppState* app_state);
void update_loop(AppState* app_state);
void draw_loop(AppState* app_state);
void draw_score(WINDOW* win, uint32_t player1_score, uint32_t player2_score);
void reset_round(AppState* app_state);
void start_round(AppState* app_state);
void draw_round_timer(WINDOW* win, Timer* timer);

#endif
