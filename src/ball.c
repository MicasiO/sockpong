#include "ball.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

void update_ball(Ball* ball, Player* player1, Player* player2, VectorInt win_size) {
    ball->pos.y += (BALL_SPEED * ball->vel.y);
    ball->pos.x += (BALL_SPEED * ball->vel.x);

    handle_wall_ball(ball, win_size);
    handle_player_ball(ball, player1);
    handle_player_ball(ball, player2);
}

void handle_wall_ball(Ball* ball, VectorInt win_size) {
    int scaled_win_y = win_size.y * FLOAT_SCALE;
    int scaled_win_x = win_size.x * FLOAT_SCALE;
    int scaled_ball_size = BALL_SIZE * FLOAT_SCALE;

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
    } else if (ball->pos.x + scaled_ball_size >= scaled_win_x - FLOAT_SCALE) {
        ball->pos.x = scaled_win_x - FLOAT_SCALE - scaled_ball_size;
        ball->vel.x = -abs(generate_random_vel(800, 1500));
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
    int32_t stuff = (int32_t)(rand() % (upper_bound - lower_bound + 1) + lower_bound);
    print_log("%d\n", stuff);
    return stuff;
}
