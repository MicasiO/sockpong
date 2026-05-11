#include "net_utils.h"
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>

bool is_valid_ipaddr(char* ip_addr) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_addr, &(sa.sin_addr));
    return result != 0;
}

void send_packet(AppState* app_state, uint8_t packet_type) {
    Packet packet;
    memset(&packet, 0, sizeof(Packet));

    packet.type = packet_type;

    app_state->packet_num++;
    packet.sequence_num = htonl(app_state->packet_num);

    packet.game_state.ball.pos.x = htonl(app_state->game_state.ball.pos.x);
    packet.game_state.ball.pos.y = htonl(app_state->game_state.ball.pos.y);
    packet.game_state.ball.vel.x = htonl(app_state->game_state.ball.vel.x);
    packet.game_state.ball.vel.y = htonl(app_state->game_state.ball.vel.y);

    packet.game_state.player1.pos.x = htonl(app_state->game_state.player1.pos.x);
    packet.game_state.player1.pos.y = htonl(app_state->game_state.player1.pos.y);
    packet.game_state.player1.vel.x = htonl(app_state->game_state.player1.vel.x);
    packet.game_state.player1.vel.y = htonl(app_state->game_state.player1.vel.y);
    packet.game_state.player1.score = htonl(app_state->game_state.player1.score);

    packet.game_state.player2.pos.x = htonl(app_state->game_state.player2.pos.x);
    packet.game_state.player2.pos.y = htonl(app_state->game_state.player2.pos.y);
    packet.game_state.player2.vel.x = htonl(app_state->game_state.player2.vel.x);
    packet.game_state.player2.vel.y = htonl(app_state->game_state.player2.vel.y);
    packet.game_state.player2.score = htonl(app_state->game_state.player2.score);

    int sent = sendto(app_state->socket_fd, &packet, sizeof(Packet), 0,
                      (struct sockaddr*)&app_state->enemy_addr, sizeof(app_state->enemy_addr));
}

bool recv_packet(AppState* app_state) {
    Packet packet;

    socklen_t enemy_addr_len = sizeof(app_state->enemy_addr);

    int received = recvfrom(app_state->socket_fd, &packet, sizeof(Packet), 0,
                            (struct sockaddr*)&app_state->enemy_addr, &enemy_addr_len);

    if (received <= 0) {
        return false;
    }

    app_state->last_packet_time = time(NULL);

    if (received < sizeof(Packet) && strncmp((char*)&packet, "start", 5) == 0) {
        if (app_state->role == PLAYER_TYPE_SERVER) {
            sendto(app_state->socket_fd, "start", 5, 0, (struct sockaddr*)&app_state->enemy_addr,
                   enemy_addr_len);
        }

        return true;
    }

    uint32_t seq = ntohl(packet.sequence_num);
    if (seq < app_state->packet_num) {
        return true;
    }

    app_state->packet_num = seq;

    if (app_state->role == PLAYER_TYPE_CLIENT) {
        app_state->game_state.ball.pos.x = ntohl(packet.game_state.ball.pos.x);
        app_state->game_state.ball.pos.y = ntohl(packet.game_state.ball.pos.y);
        app_state->game_state.ball.vel.x = ntohl(packet.game_state.ball.vel.x);
        app_state->game_state.ball.vel.y = ntohl(packet.game_state.ball.vel.y);

        app_state->game_state.player1.pos.x = ntohl(packet.game_state.player1.pos.x);
        app_state->game_state.player1.pos.y = ntohl(packet.game_state.player1.pos.y);
        app_state->game_state.player1.vel.x = ntohl(packet.game_state.player1.vel.x);
        app_state->game_state.player1.vel.y = ntohl(packet.game_state.player1.vel.y);
        app_state->game_state.player1.score = ntohl(packet.game_state.player1.score);

        app_state->game_state.player2.score = ntohl(packet.game_state.player2.score);

    } else if (app_state->role == PLAYER_TYPE_SERVER) {
        app_state->game_state.player2.pos.x = ntohl(packet.game_state.player2.pos.x);
        app_state->game_state.player2.pos.y = ntohl(packet.game_state.player2.pos.y);
        app_state->game_state.player2.vel.x = ntohl(packet.game_state.player2.vel.x);
        app_state->game_state.player2.vel.y = ntohl(packet.game_state.player2.vel.y);
    }

    return true;
}

void net_loop(AppState* app_state) {
    while (recv_packet(app_state)) {
        // drain buffer
    }

    time_t current_time = time(NULL);
    if (current_time - app_state->last_packet_time > TIMEOUT_SECONDS) {
        print_log("Opponent disconnected\n");
        app_state->running = false;
        return;
    }

    send_packet(app_state, PACKET_TYPE_GAME);
}
