//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_MAIN_H
#define PROJET_PR6_MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "connexion_serv.h"
#include "game_setup.h"
#include "../data/communication.h"
#include "user_handler.h"

#define MAX_CLI 16

struct thre {
    int sock_client[MAX_CLI];
    struct sock_and_adr *sockAndAdr;
};

#endif //PROJET_PR6_MAIN_H



