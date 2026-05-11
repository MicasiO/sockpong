#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

    if (strcmp(argv[1], "server") == 0) {
        app_state.role = PLAYER_TYPE_SERVER;
    } else if (strcmp(argv[1], "client") == 0) {
        app_state.role = PLAYER_TYPE_CLIENT;
    } else {
        fprintf(stderr, "Unknown option \"%s\". Available options: client, server\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (app_state.role == PLAYER_TYPE_CLIENT) {
        if (argc != 4) {
            fprintf(stderr, "Invalid options. Usage: 'pong client IP_ADDRESS PORT'\n");
            exit(EXIT_FAILURE);
        }

        if (!is_valid_ipaddr(argv[2])) {
            fprintf(stderr, "Invalid IP address\n");
            exit(EXIT_FAILURE);
        }

        init_client_net(&app_state, argv[2], argv[3]);

    } else if (app_state.role == PLAYER_TYPE_SERVER) {
        if (argc != 3) {
            fprintf(stderr, "Invalid options. Usage: 'pong server PORT'\n");
            exit(EXIT_FAILURE);
        }

        init_server_net(&app_state, argv[2]);
    }

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    int sx, sy;
    getmaxyx(stdscr, sy, sx);

    WINDOW* win = newwin(sy, sx, 0, 0);
    keypad(win, TRUE);

    Player player1 = {.pos = {.x = 10, .y = sy / 2}, .vel = {.x = 0, .y = 0}, .score = 0};
    Player player2 = {.pos = {.x = sx - 10, .y = sy / 2}, .vel = {.x = 0, .y = 0}, .score = 0};

    app_state.packet_num = 0;
    app_state.win = win;
    app_state.win_size.x = sx;
    app_state.win_size.y = sy;
    app_state.game_state.player1 = player1;
    app_state.game_state.player2 = player2;

    loop(&app_state);

    delwin(win);
    endwin();

    close(app_state.socket_fd);
    return 0;
}
