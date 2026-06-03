#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ball.h"
#include "game.h"
#include "network/client.h"
#include "network/net_utils.h"
#include "network/server.h"
#include "utils.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "At least one argument required\n");
        exit(EXIT_FAILURE);
    }

    AppState app_state;

    if (strcmp(argv[1], "host") == 0) {
        app_state.role = PLAYER_TYPE_SERVER;
    } else if (strcmp(argv[1], "join") == 0) {
        app_state.role = PLAYER_TYPE_CLIENT;
    } else if (strcmp(argv[1], "scan") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Invalid options. Usage: 'pong scan PORT'\n");
            exit(EXIT_FAILURE);
        }
        scan_hosts(argv[2]);
        return 0;
    } else {
        fprintf(stderr, "Unknown option \"%s\". Available options: host, join\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (app_state.role == PLAYER_TYPE_CLIENT) {
        if (argc != 4) {
            fprintf(stderr, "Invalid options. Usage: 'pong join IP_ADDRESS PORT'\n");
            exit(EXIT_FAILURE);
        }

        if (!is_valid_ipaddr(argv[2])) {
            fprintf(stderr, "Invalid IP address\n");
            exit(EXIT_FAILURE);
        }

        init_client_net(&app_state, argv[2], argv[3]);

    } else if (app_state.role == PLAYER_TYPE_SERVER) {
        if (argc != 3) {
            fprintf(stderr, "Invalid options. Usage: 'pong host PORT'\n");
            exit(EXIT_FAILURE);
        }

        init_server_net(&app_state, argv[2]);
        return 0;
    }

    srand(time(NULL));

    Player player1 = {0};
    Player player2 = {0};
    Ball ball = {0};
    ball.speed = INIT_BALL_SPEED;

    app_state.packet_num = 0;
    app_state.game_state.player1 = player1;
    app_state.game_state.player2 = player2;
    app_state.game_state.ball = ball;
    app_state.round_timer.length.tv_sec = 3;

    reset_round(&app_state);

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "sockpong");
    SetTargetFPS(30);
    SetWindowMinSize(WIN_WIDTH, WIN_HEIGHT);
    SetWindowMaxSize(WIN_WIDTH, WIN_HEIGHT);

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawRectangleLinesEx((Rectangle){-1, -1, WIN_WIDTH + 2, WIN_HEIGHT + 2}, 1, WHITE);

        if (!app_state.running) {
            break;
        }

        ClearBackground(BLACK);
        loop(&app_state);

        EndDrawing();
    }

    CloseWindow();

    close(app_state.socket_fd);
    return 0;
}
