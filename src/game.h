#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "ball.h"
#include "player.h"

typedef struct AppState AppState;

typedef struct GameState {
    Player player1;
    Player player2;
    Ball ball;
} __attribute__((packed)) GameState;

void loop(AppState* app_state);
void update_loop(AppState* app_state);
void draw_loop(AppState* app_state);

#endif
