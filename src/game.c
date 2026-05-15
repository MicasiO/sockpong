#include "game.h"
#include <ncurses.h>
#include <stdint.h>
#include <unistd.h>
#include "ball.h"
#include "figlets.h"
#include "network/net_utils.h"
#include "player.h"
#include "utils.h"

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
    mvwhline(app_state->win, 0, 0, 0, WIN_WIDTH);
    mvwhline(app_state->win, WIN_HEIGHT - 1, 0, 0, WIN_WIDTH);
    draw_score(app_state->win, game->player1.score, game->player2.score);
    draw_players(&game->player1, &game->player2, app_state->win, app_state->role);
    draw_ball(&game->ball, app_state->win);
    wnoutrefresh(app_state->win);
    doupdate();
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
                if (game->waiting_for_input) {
                    start_round(game);
                    game->waiting_for_input = 0;
                }
                handle_player_input(ch, &game->player1);
            } else if (app_state->role == PLAYER_TYPE_CLIENT) {
                if (game->waiting_for_input) {
                    game->waiting_for_input = 0;
                }
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

void draw_score(WINDOW* win, uint32_t player1_score, uint32_t player2_score) {
    draw_figlet(win, WIN_HEIGHT / 2 - 3, WIN_WIDTH / 2 - 16, figlets[player1_score]);
    draw_figlet(win, WIN_HEIGHT / 2 - 3, WIN_WIDTH / 2 + 10, figlets[player2_score]);
}

void start_round(GameState* game_state) {
    game_state->ball.vel = (VectorInt){.x = 1000, .y = 1000};
}

void reset_round(GameState* game_state) {
    game_state->player1.pos = (VectorInt){.x = 10, .y = WIN_HEIGHT / 2 - PLAYER_SIZE / 2};
    game_state->player1.vel = (VectorInt){.x = 0, .y = 0};

    game_state->player2.pos =
        (VectorInt){.x = WIN_WIDTH - 10, .y = WIN_HEIGHT / 2 - PLAYER_SIZE / 2};
    game_state->player2.vel = (VectorInt){.x = 0, .y = 0};

    game_state->ball.pos =
        (VectorInt){.x = WIN_WIDTH * FLOAT_SCALE / 2, .y = WIN_HEIGHT * FLOAT_SCALE / 2};
    game_state->ball.vel = (VectorInt){.x = 0, .y = 0};

    game_state->waiting_for_input = 1;
}
