//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_GAME_SETUP_H
#define PROJET_PR6_GAME_SETUP_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "board.h"
#include "game_handler.h"
#include "../data/communication.h"

struct game_setup_args {
    int game_sock;
    int sock_copy[4];
    int udp_port_copy;
    int multicast_port_copy;
    char *ip_copy;
    int game_mode;
};

void create_multicast_adr(GameSession *game, int multicast_port, char *multicast_address);

void *game_setup(void *args);

int create_game_socket(GameSession *game, int port);

#endif //PROJET_PR6_GAME_SETUP_H`

