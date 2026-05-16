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

    bool timer_running = !app_state->round_timer.done;

    if (timer_running) {
        get_timer(&app_state->round_timer);
        draw_round_timer(app_state->win, &app_state->round_timer);

        if (app_state->round_timer.done) {
            if (app_state->role == PLAYER_TYPE_SERVER) {
                start_round(app_state);
            }
        }
    }

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
    update_ball(app_state);
}

void draw_score(WINDOW* win, uint32_t player1_score, uint32_t player2_score) {
    wattron(win, COLOR_PAIR(SCORE_COLOR_PAIR));
    draw_figlet(win, WIN_HEIGHT / 2 - 3, WIN_WIDTH / 2 - 16, figlets[player1_score]);
    draw_figlet(win, WIN_HEIGHT / 2 - 3, WIN_WIDTH / 2 + 10, figlets[player2_score]);
    wattroff(win, COLOR_PAIR(SCORE_COLOR_PAIR));
}

void draw_round_timer(WINDOW* win, Timer* timer) {
    draw_figlet(win, 2, WIN_WIDTH / 2 - 2, figlets[(int)get_timer(timer) + 1]);
}

void start_round(AppState* app_state) {
    Ball* ball = &app_state->game_state.ball;
    ball->vel = (VectorInt){.x = 1000, .y = 1000};
}

void reset_round(AppState* app_state) {
    GameState* game_state = &app_state->game_state;
    game_state->player1.pos.x = PLAYER1_START_POS_X;
    game_state->player1.pos.y = PLAYER1_START_POS_Y;
    game_state->player1.vel = (VectorInt){.x = 0, .y = 0};

    game_state->player2.pos.x = PLAYER2_START_POS_X;
    game_state->player2.pos.y = PLAYER2_START_POS_Y;
    game_state->player2.vel = (VectorInt){.x = 0, .y = 0};

    game_state->ball.pos =
        (VectorInt){.x = WIN_WIDTH * FLOAT_SCALE / 2, .y = WIN_HEIGHT * FLOAT_SCALE / 2};
    game_state->ball.vel = (VectorInt){.x = 0, .y = 0};

    start_timer(&app_state->round_timer);
}
