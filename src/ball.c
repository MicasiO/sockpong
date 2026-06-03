#include "ball.h"
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdlib.h>
#include "game.h"
#include "network/net_utils.h"
#include "player.h"
#include "utils.h"

void update_ball(AppState* app_state) {
    GameState* game_state = &app_state->game_state;
    game_state->ball.pos.y += (game_state->ball.speed * game_state->ball.vel.y);
    game_state->ball.pos.x += (game_state->ball.speed * game_state->ball.vel.x);

    handle_wall_ball(app_state);
    handle_player_ball(&game_state->ball, &game_state->player1);
    handle_player_ball(&game_state->ball, &game_state->player2);
}

void handle_wall_ball(AppState* app_state) {
    GameState* game_state = &app_state->game_state;

    int scaled_win_y = WIN_HEIGHT * FLOAT_SCALE;
    int scaled_win_x = WIN_WIDTH * FLOAT_SCALE;
    int scaled_ball_size = BALL_SIZE * FLOAT_SCALE;

    Ball* ball = &game_state->ball;
    Player* player1 = &game_state->player1;
    Player* player2 = &game_state->player2;

    // ceiling / floor
    if (ball->pos.y - scaled_ball_size <= 0) {
        ball->pos.y = scaled_ball_size + FLOAT_SCALE * 2;
        ball->vel.y *= -1;
    } else if (ball->pos.y + scaled_ball_size >= scaled_win_y - FLOAT_SCALE) {
        ball->pos.y = scaled_win_y - 2 * FLOAT_SCALE - scaled_ball_size;
        ball->vel.y *= -1;
    }

    else if (ball->pos.x <= 0) {
        ball->pos.x = FLOAT_SCALE;

        player2->score++;
        if (player2->score == MAX_SCORE) {
            end_game(app_state);
        }
        reset_round(app_state);
    } else if (ball->pos.x + scaled_ball_size >= scaled_win_x - FLOAT_SCALE) {
        ball->pos.x = scaled_win_x - FLOAT_SCALE - scaled_ball_size;

        player1->score++;
        if (player1->score == MAX_SCORE) {
            end_game(app_state);
        }
        reset_round(app_state);
    }
}

void handle_player_ball(Ball* ball, Player* player) {
    int scaled_player_x = player->pos.x * FLOAT_SCALE;
    int scaled_player_y = player->pos.y * FLOAT_SCALE;

    int scaled_player_size_x = PLAYER_WIDTH * FLOAT_SCALE;
    int scaled_player_size_y = PLAYER_HEIGHT * FLOAT_SCALE;

    int scaled_ball_size = BALL_SIZE * FLOAT_SCALE;

    Vector2 ball_pos = (Vector2){ball->pos.x, ball->pos.y};
    Vector2 future_ball_pos =
        Vector2Add(ball_pos, Vector2Scale((Vector2){ball->vel.x, ball->vel.y}, ball->speed));

    Vector2 collision_pos;

    bool collision = CheckCollisionLines(
        ball_pos, future_ball_pos,
        (Vector2){scaled_player_x + (float)scaled_player_size_x / 2, scaled_player_y},
        (Vector2){scaled_player_x + (float)scaled_player_size_x / 2,
                  scaled_player_y + scaled_player_size_y},
        &collision_pos);

    if (collision) {
        if (ball->speed < MAX_BALL_SPEED) {
            ball->speed += BALL_SPEEP_INC;
        }

        int32_t collision_y_pos = fabsf(collision_pos.y / FLOAT_SCALE - player->pos.y);

        int32_t new_y_vel = ((collision_y_pos) * 4 * FLOAT_SCALE / PLAYER_HEIGHT) - 2 * FLOAT_SCALE;

        ball->vel.y = new_y_vel;
        ball->vel.x *= -1;

        if (ball->vel.x > 0) {
            ball->pos.x =
                scaled_player_x + scaled_player_size_x + scaled_ball_size + (1 * FLOAT_SCALE);
        } else {
            ball->pos.x = scaled_player_x - scaled_player_size_x - (1 * FLOAT_SCALE);
        }
    }
}

void draw_ball(Ball* ball) {
    int screen_x = ball->pos.x / FLOAT_SCALE;
    int screen_y = ball->pos.y / FLOAT_SCALE;
    DrawCircle(screen_x, screen_y, BALL_SIZE, WHITE);
}

int32_t generate_random_vel(int lower_bound, int upper_bound) {
    return (int32_t)(rand() % (upper_bound - lower_bound + 1) + lower_bound);
}
