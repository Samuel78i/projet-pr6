//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_USER_HANDLER_H
#define PROJET_PR6_USER_HANDLER_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <uuid/uuid.h>
#include <arpa/inet.h>
#include "board.h"
#include <unistd.h>
#include "../data/communication.h"
#include "../data/pack.h"
#include "../data/unpack.h"
#include "game_setup.h"
#include "../data/communication.h"


int add_user(int game_sock, int client_sock, uint8_t *messageClientToJoin_buffer);

int remove_user(int sock);

GameSession *findExistingGameSession();

GameSession *creationNewGameSession();

int addPlayerToSession(GameSession *session, int client_sock, struct sockaddr_in6 client_addr);

#endif //PROJET_PR6_USER_HANDLER_H

