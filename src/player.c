#include "player.h"
#include <ncurses.h>
#include <stdint.h>
#include "utils.h"

void handle_player_input(int ch, Player* player) {
    if (ch == KEY_UP) {
        player->vel.y = -1;
    } else if (ch == KEY_DOWN) {
        player->vel.y = 1;
    }
}

void update_player(Player* player, uint32_t win_height) {
    player->pos.y += (PLAYER_SPEED * player->vel.y);

    if (player->pos.y <= 0) {
        player->pos.y = 1;
        player->vel.y = 0;
    } else if (player->pos.y + PLAYER_SIZE >= win_height - 1) {
        player->pos.y = win_height - 1 - PLAYER_SIZE;
        player->vel.y = 0;
    }
}

void draw_player(Player* player, WINDOW* win) {
    for (int i = player->pos.y; i < player->pos.y + PLAYER_SIZE; i++) {
        mvwaddstr(win, i, player->pos.x, "█");
    }
}
