//
// Created by Samuel on 06/04/2024.
//

#ifndef PROJET_PR6_BOARD_H
#define PROJET_PR6_BOARD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <time.h>
#include "../data/communication.h"


#define EMPTY_SPACE 0
#define INDESTRUCTIBLE_WALL 1
#define DESTRUCTIBLE_WALL 2
#define BOMB 3
#define EXPLOSION 4
#define PLAYER_OFFSET 5

#define DESTRUCTIBLE_WALL_CHANCE 0.3 // Chance d'avoir un mur destructible dans une case vide


// structure d'un joueur
typedef struct {
    int id;
    int sock;
    int team; // 0 ou 1 pour le mode equipe 9 si solo
    int x, y;
    int isAlive;//1 pour vivant 0 pour mort
} Player;

//structure d'un bomb
typedef struct {
    int x, y;
    time_t timestamp;  //Moment ou la bombe a ete pose
    int active; // 1 pour active 0 pour non
} Bomb;


//structure pour d'une partie

typedef struct {
    Player player[MAX_PLAYER];
    int mode; // 0 pour solo 1 pour equipe
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Bomb bombs[MAX_PLAYER];//supposons une bombe pour l'instant
    struct sockaddr_in6 adr_multicast;
    struct sockaddr_in6 adr_receive;
    int sock;
} GameSession;

void create_board(GameSession *gameSession);


#endif //PROJET_PR6_BOARD_H


