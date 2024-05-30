//
// Created by Samuel on 30/03/2024.
//

#ifndef PROJET_PR6_GAME_HANDLER_H
#define PROJET_PR6_GAME_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include "board.h"
#include "game_setup.h"
#include "../data/communication.h"
#include "../data/pack.h"
#include "../data/unpack.h"

#define BOMB_TIMER 3


void *recv_action();

void *run_chat();

void process_player_action(struct message_client_played *action);

void move_player(Player *player, int dx, int dy);

void place_bomb(Player *player);

void check_bombs();

void trigger_explosion(Bomb *bomb);

void start_game(GameSession *s);

void *send_board_changes();

void copyBoard(int source[BOARD_WIDTH][BOARD_HEIGHT]);

int is_game_over();

void *send_board();

#endif // PROJET_PR6_GAME_HANDLER_H
