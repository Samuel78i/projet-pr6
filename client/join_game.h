//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_JOIN_GAME_H
#define PROJET_PR6_JOIN_GAME_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <net/if.h>

#include "in_game.h"
#include "../data/pack.h"
#include "../data/unpack.h"
#include "../data/communication.h"

void subscribe(int sock, char *ip);

int join_team_game(int sock);

int join_solo_game(int sock);

void sent_ready(int sock_serv, int mode);

int setup_udp_sock(int port);

int handle_server_response(int sock_tcp, int mode);

#endif //PROJET_PR6_JOIN_GAME_H

