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
    uint32_t waiting_for_input;
} __attribute__((packed)) GameState;

void loop(AppState* app_state);
void update_loop(AppState* app_state);
void draw_loop(AppState* app_state);
void draw_score(WINDOW* win, uint32_t player1_score, uint32_t player2_score);
void reset_round(GameState* game_state);
void start_round(GameState* game_state);

#endif
