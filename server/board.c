//
// Created by Samuel on 06/04/2024.
//

#include "board.h"

void create_board(GameSession *session) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if ((i % 2 == 0 && j % 2 == 0) || i == 0 || j == 0 || i == BOARD_HEIGHT - 1 || j == BOARD_WIDTH - 1) {
                session->board[i][j] = INDESTRUCTIBLE_WALL;
            } else {
                session->board[i][j] = EMPTY_SPACE;
            }
        }
    }

    session->board[1][1] = 5 + session->player[0].id;
    session->board[1][BOARD_WIDTH - 2] = 5 + session->player[2].id;
    session->board[BOARD_HEIGHT - 2][1] = 5 + session->player[3].id;
    session->board[BOARD_HEIGHT - 2][BOARD_WIDTH - 2] = 5 + session->player[1].id;

    for (int i = 1; i < BOARD_HEIGHT - 1; i++) {
        for (int j = 1; j < BOARD_WIDTH - 1; j++) {
            if (session->board[i][j] == EMPTY_SPACE && (double) rand() / RAND_MAX < DESTRUCTIBLE_WALL_CHANCE) {
                session->board[i][j] = DESTRUCTIBLE_WALL;
            }
        }
    }
}