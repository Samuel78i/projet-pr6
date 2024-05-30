//
// Created by Samuel on 24/04/2024.
//

#include "pack.h"

void pack_message_stcc(struct server_to_client_create *message, uint8_t *buffer) {
    buffer[0] = message->codereq;
    buffer[1] = message->id;
    buffer[2] = message->team_number;
    *((uint16_t * )(buffer + 3)) = htons(message->udp_port);
    *((uint16_t * )(buffer + 5)) = htons(message->multicast_port);
    strcpy((char *) (buffer + 7), message->multicast_address);
}

void pack_message_ctj(const struct message_client_to_join *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
}

void pack_message_client_played(const struct message_client_played *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
    *((uint16_t * )(buffer + 4)) = htons(message->num);
    buffer[6] = message->action;
}

void pack_message_server_board(const struct server_to_client_board *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
    *((uint16_t * )(buffer + 4)) = htons(message->num);
    *((uint16_t * )(buffer + 6)) = htons(message->height);
    *((uint16_t * )(buffer + 8)) = htons(message->width);
    memcpy(buffer + 10, message->grid, BOARD_WIDTH * BOARD_HEIGHT);
}

void pack_message_server_board_changes(const struct server_to_client_board_changes *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
    *((uint16_t * )(buffer + 4)) = htons(message->num % 65536);
    *((uint16_t * )(buffer + 6)) = htons(message->nb);

    for (uint8_t i = 0; i < message->nb; i++) {
        buffer[8 + i * 3] = message->square[i].row;
        buffer[8 + i * 3 + 1] = message->square[i].column;
        buffer[8 + i * 3 + 2] = message->square[i].content;
    }
}

void pack_message_fin(const struct message_fin *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
}

void pack_message_chat(const struct message_client_chat *message, uint8_t *buffer) {
    *((uint16_t *) buffer) = htons(message->codereq);
    buffer[2] = message->id;
    buffer[3] = message->eq;
    *((uint16_t * )(buffer + 4)) = htons(message->len);
    strcpy((char *) (buffer + 6), message->data);

}
