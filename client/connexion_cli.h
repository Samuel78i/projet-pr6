//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_CONNEXION_H
#define PROJET_PR6_CONNEXION_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

int connexion(char *ip, int port);

#endif //PROJET_PR6_CONNEXION_H

