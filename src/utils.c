#include "utils.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void die(const char* s) {
    endwin();
    perror(s);
    exit(1);
}

void print_log(const char* s, ...) {
    FILE* file = fopen("logs.txt", "a+");
    if (!file) {
        die("file");
    }

    va_list args;
    va_start(args, s);

    vfprintf(file, s, args);

    fflush(file);
    fclose(file);

    va_end(args);
}

void draw_figlet(WINDOW* win, int y, int x, const char* s) {
    int pos_x = x;
    int pos_y = y;

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '\n') {
            pos_y++;
            pos_x = x;
            continue;
        }
        mvwaddch(win, pos_y, pos_x, s[i]);
        pos_x++;
    }
}
