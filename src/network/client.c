#include "client.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "../utils.h"

void init_client_net(AppState* app_state, const char* ip_addr, const char* port) {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(ip_addr, port, &hints, &res) != 0) {
        die("socket get info");
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        die("socket");
    }

    app_state->socket_fd = socket_fd;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    const char* test_send_buf = "start";

    struct sockaddr_in server_addr;
    socklen_t addr_size = sizeof(server_addr);

    while (true) {
        int sent = sendto(socket_fd, test_send_buf, strlen(test_send_buf), 0, res->ai_addr,
                          res->ai_addrlen);
        if (sent == -1) {
            close(socket_fd);
            die("socket send");
        }

        char test_recv_buf[6];
        int received =
            recvfrom(socket_fd, test_recv_buf, 5, 0, (struct sockaddr*)&server_addr, &addr_size);

        if (received > 0) {
            test_recv_buf[received] = '\0';
            if (strcmp(test_recv_buf, "start") == 0) {
                app_state->running = true;
                break;
            }

        } else {
            printf("client: request timed out. retrying...\n");
        }
    }

    app_state->enemy_addr = server_addr;
    app_state->last_packet_time = time(NULL);

    tv.tv_sec = 0;
    tv.tv_usec = 10;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    freeaddrinfo(res);

    print_log("client: received packet, connection established\n");
}

void scan_hosts(const char* port) {
    printf("Scanning...\n");

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo("255.255.255.255", port, &hints, &res) != 0) {
        die("socket get info");
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        die("socket");
    }

    int broadcast_enable = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable));

    const char* test_send_buf = "scan";

    struct sockaddr_in server_addr;
    socklen_t addr_size = sizeof(server_addr);

    for (int i = 0; i < 20; i++) {
        int sent = sendto(socket_fd, test_send_buf, strlen(test_send_buf), 0, res->ai_addr,
                          res->ai_addrlen);
        if (sent == -1) {
            close(socket_fd);
            die("socket send");
        }
    }

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    char test_recv_buf[6];
    int received =
        recvfrom(socket_fd, test_recv_buf, 4, 0, (struct sockaddr*)&server_addr, &addr_size);

    if (received > 0) {
        test_recv_buf[received] = '\0';
        if (strcmp(test_recv_buf, "scan") == 0) {
            char ip_str[INET_ADDRSTRLEN];
            printf("%s\n", inet_ntop(AF_INET, &(server_addr.sin_addr), ip_str, INET_ADDRSTRLEN));
        }
    } else {
        printf("No hosts found\n");
    }

    freeaddrinfo(res);
    close(socket_fd);
}
