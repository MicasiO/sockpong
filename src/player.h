#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>
#include <stdint.h>
#include "utils.h"

#define PLAYER_SPEED 1
#define PLAYER_SIZE 5

#define PLAYER1_START_POS_X 10
#define PLAYER1_START_POS_Y WIN_HEIGHT / 2

#define PLAYER2_START_POS_X WIN_WIDTH - 10
#define PLAYER2_START_POS_Y WIN_HEIGHT / 2

typedef struct GameState GameState;

typedef enum player_type { PLAYER_TYPE_CLIENT, PLAYER_TYPE_SERVER } player_type;

typedef struct Player {
    VectorInt pos;
    VectorInt vel;
    uint32_t score;
} __attribute__((packed)) Player;

void handle_player_input(int ch, Player* player);
void update_player(Player* player, uint32_t win_height);
void draw_players(Player* player1, Player* player2, WINDOW* win, player_type role);

#endif
