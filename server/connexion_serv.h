//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_CONNEXION_H
#define PROJET_PR6_CONNEXION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "game_handler.h"
#include "../data/communication.h"


struct sock_and_adr *connexion(int port);

#endif //PROJET_PR6_CONNEXION_H


