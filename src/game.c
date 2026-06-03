#include "game.h"
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ball.h"
#include "network/net_utils.h"
#include "player.h"
#include "utils.h"

void loop(AppState* app_state) {
    net_loop(app_state);
    draw_loop(app_state);
    update_loop(app_state);
    usleep(33333);
}

void draw_loop(AppState* app_state) {
    GameState* game = &app_state->game_state;
    draw_score(game->player1.score, game->player2.score);
    draw_players(&game->player1, &game->player2);
    draw_ball(&game->ball);
}

void update_loop(AppState* app_state) {
    GameState* game = &app_state->game_state;

    bool timer_running = !app_state->round_timer.done;

    if (timer_running) {
        get_timer(&app_state->round_timer);
        draw_round_timer(&app_state->round_timer);

        if (app_state->round_timer.done) {
            if (app_state->role == PLAYER_TYPE_SERVER) {
                start_round(app_state);
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        app_state->running = false;
    }

    if (app_state->role == PLAYER_TYPE_SERVER) {
        handle_player_input(&game->player1);
    } else if (app_state->role == PLAYER_TYPE_CLIENT) {
        handle_player_input(&game->player2);
    }

    update_player(&game->player1);
    update_player(&game->player2);
    update_ball(app_state);
}

void draw_score(uint32_t player1_score, uint32_t player2_score) {
    const char* score_text = TextFormat("%d - %d", player1_score, player2_score);
    int score_text_width = MeasureText(score_text, 64);
    DrawText(score_text, WIN_WIDTH / 2 - score_text_width / 2, 50, 64, GRAY);
}

void draw_round_timer(Timer* timer) {
    const char* round_timer_text = TextFormat("%d", (int)get_timer(timer) + 1);

    int round_timer_text_width = MeasureText(round_timer_text, 72);
    DrawText(round_timer_text, WIN_WIDTH / 2 - round_timer_text_width / 2, WIN_HEIGHT - 150, 72,
             LIGHTGRAY);
}

void start_round(AppState* app_state) {
    Ball* ball = &app_state->game_state.ball;
    bool left_right = rand() & 1;

    ball->speed = INIT_BALL_SPEED;
    ball->vel.x = left_right ? -1 * FLOAT_SCALE : 1 * FLOAT_SCALE;
    ball->vel.y = 1;
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

void end_game(AppState* app_state) {
    app_state->running = false;

    // EndDrawing();
    // CloseWindow();
    // close(app_state->socket_fd);

    bool player1_won = app_state->game_state.player1.score > app_state->game_state.player2.score;

    if (player1_won) {
        if (app_state->role == PLAYER_TYPE_SERVER) {
            printf("You won\n");
        } else {
            printf("You lost\n");
        }
    } else {
        if (app_state->role == PLAYER_TYPE_SERVER) {
            printf("You lost\n");
        } else {
            printf("You won\n");
        }
    }
}
