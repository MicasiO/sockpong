#include "ball.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include "game.h"
#include "network/net_utils.h"
#include "utils.h"

void update_ball(AppState* app_state) {
    GameState* game_state = &app_state->game_state;
    game_state->ball.pos.y += (BALL_SPEED * game_state->ball.vel.y);
    game_state->ball.pos.x += (BALL_SPEED * game_state->ball.vel.x);

    handle_wall_ball(app_state);
    handle_player_ball(&game_state->ball, &game_state->player1);
    handle_player_ball(&game_state->ball, &game_state->player2);
}

void handle_wall_ball(AppState* app_state) {
    GameState* game_state = &app_state->game_state;

    int scaled_win_y = app_state->win_size.y * FLOAT_SCALE;
    int scaled_win_x = app_state->win_size.x * FLOAT_SCALE;
    int scaled_ball_size = BALL_SIZE * FLOAT_SCALE;

    Ball* ball = &game_state->ball;
    Player* player1 = &game_state->player1;
    Player* player2 = &game_state->player2;

    if (ball->pos.y - scaled_ball_size <= 0) {
        ball->pos.y = FLOAT_SCALE * 2;
        ball->vel.y = abs(generate_random_vel(300, 600));
    } else if (ball->pos.y + scaled_ball_size >= scaled_win_y - FLOAT_SCALE) {
        ball->pos.y = scaled_win_y - FLOAT_SCALE - scaled_ball_size;
        ball->vel.y = -abs(generate_random_vel(300, 600));
    }

    else if (ball->pos.x <= 0) {
        ball->pos.x = FLOAT_SCALE;
        ball->vel.x = abs(generate_random_vel(800, 1500));
        if (player2->score < MAX_SCORE) {
            player2->score++;
        }
        reset_round(app_state);
    } else if (ball->pos.x + scaled_ball_size >= scaled_win_x - FLOAT_SCALE) {
        ball->pos.x = scaled_win_x - FLOAT_SCALE - scaled_ball_size;
        ball->vel.x = -abs(generate_random_vel(800, 1500));
        if (player1->score < MAX_SCORE) {
            player1->score++;
        }
        reset_round(app_state);
    }
}

void handle_player_ball(Ball* ball, Player* player) {
    int scaled_player_x = player->pos.x * FLOAT_SCALE;
    int scaled_player_y = player->pos.y * FLOAT_SCALE;
    int scaled_player_size = PLAYER_SIZE * FLOAT_SCALE;
    int scaled_ball_size = BALL_SIZE * FLOAT_SCALE;

    bool overlap_x = (ball->pos.x + scaled_ball_size >= scaled_player_x) &&
                     (ball->pos.x <= scaled_player_x + FLOAT_SCALE);

    bool overlap_y = (ball->pos.y + scaled_ball_size >= scaled_player_y) &&
                     (ball->pos.y <= scaled_player_y + scaled_player_size);

    if (overlap_x && overlap_y) {
        ball->vel.x = ball->vel.x > 0 ? -abs(generate_random_vel(1200, 1500))
                                      : generate_random_vel(1200, 1500);

        if (ball->vel.x > 0) {
            ball->pos.x = scaled_player_x + FLOAT_SCALE + 1;
        } else {
            ball->pos.x = scaled_player_x - scaled_ball_size - 1;
        }
    }
}

void draw_ball(Ball* ball, WINDOW* win) {
    wattron(win, COLOR_PAIR(BALL_COLOR_PAIR));
    int screen_x = ball->pos.x / FLOAT_SCALE;
    int screen_y = ball->pos.y / FLOAT_SCALE;
    mvwaddstr(win, screen_y, screen_x, "██");
    wattroff(win, COLOR_PAIR(BALL_COLOR_PAIR));
}

int32_t generate_random_vel(int lower_bound, int upper_bound) {
    return (int32_t)(rand() % (upper_bound - lower_bound + 1) + lower_bound);
}
