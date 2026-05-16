#ifndef NET_UTILS_H
#define NET_UTILS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <time.h>
#include "../game.h"

#define PACKET_TYPE_CONNECT 0
#define PACKET_TYPE_GAME 1
#define TIMEOUT_SECONDS 2

typedef struct GameState GameState;

typedef struct AppState {
    player_type role;
    int socket_fd;
    struct sockaddr_in enemy_addr;
    uint32_t packet_num;
    bool running;
    Timer round_timer;
    WINDOW* win;
    VectorInt win_size;
    GameState game_state;
    time_t last_packet_time;
} AppState;

typedef struct Packet {
    uint8_t type;
    uint32_t sequence_num;
    GameState game_state;
} __attribute__((packed)) Packet;

bool is_valid_ipaddr(char* ip_addr);

void send_init_packet();
void recv_init_packet();

void send_packet(AppState* app_state, uint8_t packet_type);
bool recv_packet(AppState* app_state);

void net_loop(AppState* app_state);

#endif
