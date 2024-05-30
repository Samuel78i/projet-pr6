//
// Created by Samuel on 24/04/2024.
//

#ifndef PROJET_PR6_UNPACK_H
#define PROJET_PR6_UNPACK_H

#include "communication.h"

void unpack_message_stcc(uint8_t *buffer, struct server_to_client_create *message);

void unpack_message_ctj(const uint8_t *buffer, struct message_client_to_join *message);

void unpack_message_client_played(const uint8_t *buffer, struct message_client_played *message);

void unpack_message_server_board(const uint8_t *buffer, struct server_to_client_board *message);

void unpack_message_server_board_changes(const uint8_t *buffer, struct server_to_client_board_changes *message);

void unpack_message_fin(const uint8_t *buffer, struct message_fin *message);

void unpack_message_chat(const uint8_t *buffer, struct message_client_chat *message);

#endif //PROJET_PR6_UNPACK_H

