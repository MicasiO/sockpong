#include "player.h"
#include <raylib.h>
#include <stdint.h>
#include "utils.h"

void handle_player_input(Player* player) {
    if (IsKeyDown(KEY_UP)) {
        player->vel.y = -1;
    } else if (IsKeyDown(KEY_DOWN)) {
        player->vel.y = 1;
    } else {
        player->vel.y = 0;
    }
}

void update_player(Player* player) {
    player->pos.y += (PLAYER_SPEED * player->vel.y);

    if (player->pos.y <= 0) {
        player->pos.y = 1;
        player->vel.y = 0;
    } else if (player->pos.y + PLAYER_HEIGHT >= WIN_HEIGHT - 1) {
        player->pos.y = WIN_HEIGHT - 1 - PLAYER_HEIGHT;
        player->vel.y = 0;
    }
}

void draw_players(Player* player1, Player* player2) {
    // TODO: perhaps implement flip screen for client

    DrawRectangle(player1->pos.x, player1->pos.y, 10, PLAYER_HEIGHT, BLUE);
    DrawRectangle(player2->pos.x, player2->pos.y, 10, PLAYER_HEIGHT, RED);
}
