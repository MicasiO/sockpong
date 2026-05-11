#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>
#include <stdint.h>
#include "utils.h"

typedef struct GameState GameState;

typedef struct Player {
    Vector pos;
    Vector vel;
    uint32_t score;
} __attribute__((packed)) Player;

void handle_player_input(int ch, Player* player);
void update_player(Player* player, uint32_t win_height);
void draw_player(Player* player, WINDOW* win);

#endif
