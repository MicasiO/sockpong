#ifndef CLIENT_H
#define CLIENT_H

#include "net_utils.h"

void init_client_net(AppState* app_state, const char* ip_addr, const char* port);
void scan_hosts(const char* port);

#endif
