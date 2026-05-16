#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    }

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < WIN_HEIGHT || max_x < WIN_WIDTH) {
        close(app_state.socket_fd);
        endwin();
        fprintf(stderr, "Increase your terminal window size to play\n");
        exit(EXIT_FAILURE);
    }

    if (has_colors() == FALSE) {
        close(app_state.socket_fd);
        endwin();
        fprintf(stderr, "Your terminal does not support colors\n");
        exit(EXIT_FAILURE);
    }

    start_color();
    use_default_colors();
    init_color(COLOR_GRAY, 500, 500, 500);
    init_pair(BALL_COLOR_PAIR, COLOR_RED, COLOR_RED);
    init_pair(SCORE_COLOR_PAIR, COLOR_GRAY, -1);

    srand(time(NULL));

    WINDOW* win = newwin(WIN_HEIGHT, WIN_WIDTH, 0, 0);
    keypad(win, TRUE);

    Player player1 = {0};
    Player player2 = {0};
    Ball ball = {0};

    app_state.packet_num = 0;
    app_state.win = win;
    app_state.win_size.x = WIN_WIDTH;
    app_state.win_size.y = WIN_HEIGHT;
    app_state.game_state.player1 = player1;
    app_state.game_state.player2 = player2;
    app_state.game_state.ball = ball;
    app_state.round_timer.length.tv_sec = 3;

    reset_round(&app_state);

    loop(&app_state);

    delwin(win);
    endwin();

    close(app_state.socket_fd);
    return 0;
}
