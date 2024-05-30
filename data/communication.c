//
// Created by Samuel on 06/04/2024.
//

#include "communication.h"


struct message_client_to_join create_message_client_to_join(uint16_t code) {
    struct message_client_to_join message;
    message.codereq = code;
    message.id = 0;
    message.eq = 0;
    return message;
}

struct message_client_played create_message_client_udp(uint16_t code, uint8_t id,
                                                       uint8_t eq, uint16_t num, uint8_t action) {

    struct message_client_played message;
    message.codereq = code;
    message.id = id;
    message.eq = eq;
    message.num = num;
    message.action = action;
    return message;
}

struct message_client_chat create_message_client_chat(uint16_t code, uint8_t id,
                                                      uint8_t eq, uint16_t len, char *data) {

    struct message_client_chat message;
    message.codereq = code;
    message.id = id;
    message.eq = eq;
    message.len = strlen(data);
    message.data = malloc(message.len + 1);
    strcpy(message.data, data);
    return message;
}

struct server_to_client_create create_server_to_client_create(uint8_t code, uint8_t id, uint8_t team_number,
                                                              uint16_t udp_port, uint16_t multicast_port,
                                                              char *multicast_address) {

    struct server_to_client_create message;
    message.codereq = code;
    message.id = id;
    message.team_number = team_number;
    message.udp_port = udp_port;
    message.multicast_port = multicast_port;
    message.multicast_address = multicast_address;
    return message;
}


struct server_to_client_board create_server_to_client_board(uint16_t num) {
    struct server_to_client_board message;
    // Initialisation des données du message
    message.codereq = 11; // Codereq en big endian
    message.id = 0;
    message.eq = 0;
    message.num = num; // Numéro du message en big endian
    message.height = BOARD_HEIGHT; // Hauteur de la grille en big endian
    message.width = BOARD_WIDTH; // Largeur de la grille en big endian
    return message;
}

void fill_grid_data(struct server_to_client_board *message, int board[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            message->grid[i * BOARD_HEIGHT + j] = (uint8_t) board[i][j]; // Conversion en uint8_t
        }
    }
}

struct server_to_client_board_changes create_server_to_client_board_changes(uint8_t n, uint8_t nb, struct square *s) {
    struct server_to_client_board_changes message;
    message.codereq = 12;
    message.id = 0;
    message.eq = 0;
    message.num = n;
    message.nb = nb;
    message.square = s;
    return message;
}

