#include "server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "../utils.h"
#include "net_utils.h"

void init_server_net(AppState* app_state, const char* port) {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res) == -1) {
        die("socket get info");
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        die("socket");
    }

    app_state->socket_fd = socket_fd;

    if (bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        close(socket_fd);
        die("socket bind");
    }

    freeaddrinfo(res);

    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (true) {
        char test_recv_buf[6];
        int received =
            recvfrom(socket_fd, test_recv_buf, 5, 0, (struct sockaddr*)&client_addr, &addr_size);

        if (received > 0) {
            test_recv_buf[received] = '\0';
            if (strcmp(test_recv_buf, "start") == 0) {
                app_state->running = true;

                sendto(socket_fd, "start", 5, 0, (struct sockaddr*)&client_addr, addr_size);
                break;
            }
        }
    }

    app_state->enemy_addr = client_addr;
    app_state->last_packet_time = time(NULL);

    tv.tv_sec = 0;
    tv.tv_usec = 10;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    print_log("server: received packet, connection established\n");
}
