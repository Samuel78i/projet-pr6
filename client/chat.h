//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_CHAT_H
#define PROJET_PR6_CHAT_H

#include "../data/communication.h"
#include "in_game.h"

#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

void send_message_chat(int sock, uint16_t eq, uint8_t id);

#endif // PROJET_PR6_CHAT_H
