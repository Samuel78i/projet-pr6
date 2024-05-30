//
// Created by Samuel on 06/04/2024.
//

#ifndef PROJET_PR6_COMMUNICATION_H
#define PROJET_PR6_COMMUNICATION_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define TEXT_SIZE 255
#define MAX_PLAYER 4
#define BOARD_HEIGHT 10
#define BOARD_WIDTH 10

// CLIENT
#define IP_SERV "::1"


#define SIZE_MSG 512

struct sock_and_adr {
    int sock;
    struct sockaddr_in6 adr;
};

struct message_client_to_join {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
};//code,id,eq

struct message_client_played {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
    uint16_t num;
    uint8_t action;
};//code,id,eq,num,action

struct message_client_chat {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
    uint16_t len;
    char *data;
};//code,id,eq,len,data

struct server_to_client_create {
    uint8_t codereq;
    uint8_t id;
    uint8_t team_number;
    uint16_t udp_port;
    uint16_t multicast_port;
    char *multicast_address;
};//code,id,team_number,upd_port,multicast_port,multicast_address

struct server_to_client_board {
    uint16_t codereq;                         // Code de requête
    uint8_t id;                               // ID
    uint8_t eq;                               // EQ
    uint16_t num;                             // Numéro du message
    uint16_t height;                          // Hauteur de la grille
    uint16_t width;                           // Largeur de la grille
    uint8_t grid[BOARD_WIDTH * BOARD_HEIGHT]; // Données de la grille
};//code,id,eq,num,height,width,grid

struct square {
    uint8_t row;
    uint8_t column;
    uint8_t content;
};

struct server_to_client_board_changes {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
    uint16_t num;
    uint16_t nb;
    struct square *square;
};//code,id,eq,num,nb,square

struct message_fin {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
};//code,id,eq

struct message_client_to_join create_message_client_to_join(uint16_t code);

struct message_client_played
create_message_client_udp(uint16_t code, uint8_t id, uint8_t eq, uint16_t num, uint8_t action);

struct message_client_chat create_message_client_chat(uint16_t code, uint8_t id, uint8_t eq, uint16_t len, char *data);

struct server_to_client_create create_server_to_client_create(uint8_t code, uint8_t id,
                                                              uint8_t team_number, uint16_t udp_port,
                                                              uint16_t multicast_port, char *multicast_address);

struct server_to_client_board create_server_to_client_board(uint16_t num);

struct server_to_client_board_changes create_server_to_client_board_changes(uint8_t n, uint8_t nb, struct square *s);

void fill_grid_data(struct server_to_client_board *message, int board[BOARD_WIDTH][BOARD_HEIGHT]);

#endif // PROJET_PR6_COMMUNICATION_H
