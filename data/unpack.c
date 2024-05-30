//
// Created by Samuel on 24/04/2024.
//

#include <printf.h>
#include "unpack.h"

void unpack_message_stcc(uint8_t *buffer, struct server_to_client_create *message) {
    message->codereq = buffer[0];
    message->id = buffer[1];
    message->team_number = buffer[2];
    message->udp_port = ntohs(*((uint16_t * )(buffer + 3)));
    message->multicast_port = ntohs(*((uint16_t * )(buffer + 5)));
    message->multicast_address = strdup((char *) (buffer + 7));
}

void unpack_message_ctj(const uint8_t *buffer, struct message_client_to_join *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
}

void unpack_message_client_played(const uint8_t *buffer, struct message_client_played *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
    message->num = ntohs(*((uint16_t * )(buffer + 4)));
    message->action = buffer[6];
}

void unpack_message_server_board(const uint8_t *buffer, struct server_to_client_board *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
    message->num = ntohs(*((uint16_t * )(buffer + 4)));
    message->height = ntohs(*((uint16_t * )(buffer + 6)));
    message->width = ntohs(*((uint16_t * )(buffer + 8)));
    memcpy(message->grid, buffer + 10, BOARD_WIDTH * BOARD_HEIGHT);
}

void unpack_message_server_board_changes(const uint8_t *buffer, struct server_to_client_board_changes *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
    message->num = ntohs(*((uint16_t * )(buffer + 4)));
    message->nb = ntohs(*((uint16_t * )(buffer + 6)));

    message->square = (struct square *) malloc(message->nb * sizeof(struct square));
    for (uint8_t i = 0; i < message->nb; i++) {
        message->square[i].row = buffer[8 + i * 3];
        message->square[i].column = buffer[8 + i * 3 + 1];
        message->square[i].content = buffer[8 + i * 3 + 2];
    }
}

void unpack_message_fin(const uint8_t *buffer, struct message_fin *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
}

void unpack_message_chat(const uint8_t *buffer, struct message_client_chat *message) {
    message->codereq = ntohs(*((uint16_t *) buffer));
    message->id = buffer[2];
    message->eq = buffer[3];
    message->len = ntohs(*((uint16_t * )(buffer + 4)));
    message->data = strdup((char *) (buffer + 6));
}

