//
// Created by Samuel on 24/04/2024.
//

#ifndef PROJET_PR6_PACK_H
#define PROJET_PR6_PACK_H

#include "communication.h"


void pack_message_stcc(struct server_to_client_create *message, uint8_t *buffer);

void pack_message_ctj(const struct message_client_to_join *message, uint8_t *buffer);

void pack_message_client_played(const struct message_client_played *message, uint8_t *buffer);

void pack_message_server_board(const struct server_to_client_board *message, uint8_t *buffer);

void pack_message_server_board_changes(const struct server_to_client_board_changes *message, uint8_t *buffer);

void pack_message_fin(const struct message_fin *message, uint8_t *buffer);

void pack_message_chat(const struct message_client_chat *message, uint8_t *buffer);

#endif //PROJET_PR6_PACK_H

