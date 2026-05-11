#include "game.h"
#include <ncurses.h>
#include <unistd.h>
#include "ball.h"
#include "network/net_utils.h"

void loop(AppState* app_state) {
    nodelay(app_state->win, TRUE);
    while (app_state->running) {
        net_loop(app_state);
        draw_loop(app_state);
        update_loop(app_state);
        usleep(33333);
    }
}

void draw_loop(AppState* app_state) {
    GameState* game = &app_state->game_state;
    werase(app_state->win);
    box(app_state->win, 0, 0);
    draw_players(&game->player1, &game->player2, app_state->win, app_state->role);
    draw_ball(&game->ball, app_state->win);
}

void update_loop(AppState* app_state) {
    GameState* game = &app_state->game_state;
    int ch = wgetch(app_state->win);
    switch (ch) {
        case 27:  // esc
            app_state->running = false;
            break;
        case KEY_UP:
        case KEY_DOWN:
            if (app_state->role == PLAYER_TYPE_SERVER) {
                handle_player_input(ch, &game->player1);
            } else if (app_state->role == PLAYER_TYPE_CLIENT) {
                handle_player_input(ch, &game->player2);
            }
            break;
        default:
            break;
    }

    update_player(&game->player1, app_state->win_size.y);
    update_player(&game->player2, app_state->win_size.y);
    update_ball(&game->ball, &game->player1, &game->player2, app_state->win_size);
}
