#include "utils.h"
#include <ncurses.h>
#include <stdlib.h>

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
