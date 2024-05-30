//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_IN_GAME_H
#define PROJET_PR6_IN_GAME_H

#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <ncurses.h>
#include "../data/communication.h"
#include "../data/pack.h"
#include "../data/unpack.h"
#include "chat.h"


#define TEXT_SIZE 255


struct game_data {
    uint16_t code;
    uint8_t id;
    uint8_t eq;
    int sock;
    int sock_tcp;
    struct sockaddr_in6 adr;
};
typedef struct board {
    char *grid;
    int w;
    int h;
} board;

typedef struct line {
    char data[TEXT_SIZE];
    int cursor;
} line;


_Noreturn void *send_action(void *arg);

_Noreturn void *recv_mess(void *arg);

void game_start(int sock_serv, int sock_udp, struct sockaddr_in6 in6, uint8_t id, uint8_t codereq, uint8_t team_number);

#endif //PROJET_PR6_IN_GAME_H
