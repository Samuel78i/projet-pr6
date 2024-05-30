//
// Created by Samuel on 30/03/2024.
//

#include "game_handler.h"

GameSession *session;

int last_board[BOARD_HEIGHT][BOARD_WIDTH];

void start_game(GameSession *s) {

    printf("LAUNCHING GAME \n");
    session = s;

    pthread_t thread[4];

    pthread_create(&thread[0], NULL, recv_action, NULL);
    pthread_create(&thread[1], NULL, send_board, NULL);
    pthread_create(&thread[2], NULL, send_board_changes, NULL);
    pthread_create(&thread[3], NULL, run_chat, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);
    pthread_join(thread[3], NULL);

    printf("GAME OVER\n");
}

void *run_chat() {
    fd_set readfds;
    int max_sd = 4; // socket client max +1 =4+1=5
    int gameover;
    while ((gameover = is_game_over()) == -1) {
        check_bombs();

        FD_ZERO(&readfds);
        for (int i = 0; i < max_sd; i++) {
            FD_SET(session->player[i].sock, &readfds);
        }

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);
        if (activity < 0) {
            perror("select erro");
            exit(EXIT_FAILURE);
        } else if (activity > 0) {
            for (int i = 0; i < max_sd; i++) {
                if (FD_ISSET(session->player[i].sock, &readfds)) {
                    struct message_client_chat mss;
                    size_t total = recv(session->player[i].sock, &mss, sizeof(mss), 0);
                    while (total < sizeof(mss)) {
                        size_t n = recv(session->player[i].sock, &mss + total, sizeof(mss) - total, 0);
                        total += n;
                    }
                    if (mss.codereq == 13) {
                        // mss envoie à tous les j!=i
                        for (int j = 0; j < max_sd; j++) {
                            total = send(session->player[j].sock, &mss, sizeof(mss), 0);
                            while (total < sizeof(mss)) {
                                size_t n = send(session->player[j].sock, &mss + total, sizeof(mss) - total, 0);
                                total += n;
                            }
                        }
                    }
                    if (mss.codereq == 14) {
                        int teamate;
                        if (i == 0) {
                            teamate = 1;
                        }
                        if (i == 1) {
                            teamate = 0;
                        }
                        if (i == 2) {
                            teamate = 3;
                        }
                        if (i == 3) {
                            teamate = 2;
                        }
                        total = send(session->player[teamate].sock, &mss, sizeof(mss), 0);
                        while (total < sizeof(mss)) {
                            size_t n = send(session->player[teamate].sock, &mss + total, sizeof(mss) - total, 0);
                            total += n;
                        }
                    }
                }
            }
        }
    }

    // envoie la fin en tcp à tous les joueurs et ferme socket et free les memoire
    struct message_fin mss_fin;
    if (gameover == 4) {
        // equipe 0  1
        mss_fin.codereq = 16;
        mss_fin.id = 0; // peu import
        mss_fin.eq = 1;
    } else if (gameover == 5) {
        // equipe 2  3
        mss_fin.codereq = 16;
        mss_fin.id = 0;
        mss_fin.eq = 2;
    } else {
        // mode solo
        mss_fin.codereq = 15;
        mss_fin.id = gameover;
        mss_fin.eq = 0; // peu import
    }

    uint8_t message_buffer[4];
    pack_message_fin(&mss_fin, message_buffer);

    for (int i = 0; i < 4; i++) {
        send(session->player[i].sock, message_buffer, sizeof(message_buffer), 0);
    }

    return NULL;
}

void *recv_action() {
    while (is_game_over() == -1) {
        fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(session->sock, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(session->sock + 1, &readfds, NULL, NULL, &timeout);

        if (activity < 0) {
            perror("select error");
            break;
        } else if (activity > 0) {
            if (FD_ISSET(session->sock, &readfds)) {
                uint8_t messageClientPlayed_buffer[SIZE_MSG + 1];
                socklen_t len = sizeof(session->adr_receive);
                ssize_t r = recvfrom(session->sock, messageClientPlayed_buffer, sizeof(messageClientPlayed_buffer), 0,
                                     (struct sockaddr *) &session->adr_receive, &len);
                if (r < 0) {
                    perror("erreur recvfrom\n");
                } else {
                    struct message_client_played messageClientPlayed;
                    unpack_message_client_played(messageClientPlayed_buffer, &messageClientPlayed);
                    process_player_action(&messageClientPlayed);
                }
            }
        }
    }

    return NULL;
}

int is_game_over() {
    if ((session->player[0].isAlive == false && session->player[1].isAlive == false) &&
        (session->player[2].isAlive == false && session->player[3].isAlive == false)) {
        // 6
        return 6;
    }

    if (session->mode == 1) {
        // TEAM GAME MODE
        if ((session->player[2].isAlive == false && session->player[3].isAlive == false) &&
            (session->player[0].isAlive == true || session->player[1].isAlive == true)) {
            // 4
            return 4;
        }
        if ((session->player[0].isAlive == false && session->player[1].isAlive == false) &&
            (session->player[2].isAlive == true || session->player[3].isAlive == true)) {
            // 5
            return 5;
        }
    } else {
        // SOLO GAME MODE

        // PLAYER 1 WINS
        if ((session->player[0].isAlive == true && session->player[1].isAlive == false) &&
            (session->player[2].isAlive == false && session->player[3].isAlive == false)) {
            // 0
            return 0;
        }

        // PLAYER 2 WINS
        if ((session->player[0].isAlive == false && session->player[1].isAlive == true) &&
            (session->player[2].isAlive == false && session->player[3].isAlive == false)) {
            // 1
            return 1;
        }

        // PLAYER 3 WINS
        if ((session->player[0].isAlive == false && session->player[1].isAlive == false) &&
            (session->player[2].isAlive == true && session->player[3].isAlive == false)) {
            // 2
            return 2;
        }

        // PLAYER 4 WINS
        if ((session->player[0].isAlive == false && session->player[1].isAlive == false) &&
            (session->player[2].isAlive == false && session->player[3].isAlive == true)) {
            // 3
            return 3;
        }
    }
    //-1
    return -1;
}

void move_player(Player *player, int dx, int dy) {
    int new_x = player->x + dx;
    int new_y = player->y + dy;

    if (new_x > 0 && new_x < BOARD_HEIGHT && new_y > 0 && new_y < BOARD_WIDTH &&
        session->board[new_y][new_x] == EMPTY_SPACE) {

        session->board[player->y][player->x] = EMPTY_SPACE;

        for (int i = 0; i < 4; i++) {
            if (session->bombs[i].x == player->x && session->bombs[i].y == player->y) {
                session->board[player->y][player->x] = BOMB;
            }
        }

        player->x = new_x;
        player->y = new_y;

        session->board[new_y][new_x] = PLAYER_OFFSET + player->id;
    }
}

void place_bomb(Player *player) {
    int x = player->x;
    int y = player->y;

    if (!session->bombs[player->id].active) {
        session->board[y][x] = BOMB;
        // Trouver un wall libre pour la bombe
        for (int i = 0; i < MAX_PLAYER; i++) {
            if (session->bombs[i].x == -1 &&
                session->bombs[i].y == -1) { // Supposons que -1, -1 indique une bombe non active
                session->bombs[i].x = x;
                session->bombs[i].y = y;
                session->bombs[i].active = 1;
                session->bombs[i].timestamp = time(NULL); // Timestamp actuel
                printf("Player %d placed a bomb at (%d, %d)\n", player->id, x, y);
                break;
            }
        }
    } else {
        printf("Player %d cannot place bomb at (%d, %d)\n", player->id, x, y);
    }
}

void process_player_action(struct message_client_played *action) {
    if (!session->player[action->id - 1].isAlive) {
        printf("Player %d is not alive and cannot move.\n", session->player[action->id - 1].id);
        return;
    }

    if (action->action == 0) {
        move_player(&session->player[action->id - 1], 0, -1);
    } else if (action->action == 1) {
        move_player(&session->player[action->id - 1], 1, 0);
    } else if (action->action == 2) {
        move_player(&session->player[action->id - 1], 0, 1);
    } else if (action->action == 3) {
        move_player(&session->player[action->id - 1], -1, 0);
    } else if (action->action == 4) {
        place_bomb(&session->player[action->id - 1]);
    } else if (action->action == 5) {
        // TODO cancel previous action
    }

}

void trigger_explosion(Bomb *bomb) {
    if (!bomb->active)
        return;

    int x = bomb->x;
    int y = bomb->y;

    // Définir les cases affectées par l'explosion
    static const int directions[4][2] = {{0,  1},
                                         {1,  0},
                                         {0,  -1},
                                         {-1, 0}}; // Sud, Est, Nord, Ouest

    session->board[y][x] = EMPTY_SPACE;              // La case de la bombe devient une explosion

    for (int dir = 0; dir < 4; dir++) {
        for (int step = 1; step <= 2; step++) { // Portée de l'explosion
            int nx = x + directions[dir][0] * step;
            int ny = y + directions[dir][1] * step;

            if (nx >= 0 && nx < BOARD_WIDTH && ny >= 0 && ny < BOARD_HEIGHT) {
                if (session->board[ny][nx] == INDESTRUCTIBLE_WALL) {
                    break; // Un mur indestructible bloque l'explosion
                }
                if (session->board[ny][nx] > 4) {
                    session->player[session->board[ny][nx] - PLAYER_OFFSET].isAlive = false;
                }

                session->board[ny][nx] = EMPTY_SPACE;

                if (session->board[ny][nx] == DESTRUCTIBLE_WALL) {
                    break; // Un mur destructible est détruit et bloque l'explosion
                }

            }
        }
    }
    bomb->x = -1;
    bomb->y = -1;
    bomb->active = 0; // La bombe a explosé et n'est plus active
    printf("Bomb exploded at (%d, %d)\n", x, y);
}

void check_bombs() {
    time_t current_time = time(NULL);

    for (int i = 0; i < MAX_PLAYER; i++) {

        if (session->bombs[i].active && (difftime(current_time, session->bombs[i].timestamp) >= BOMB_TIMER)) {
            trigger_explosion(&session->bombs[i]);
        }
    }
}


void copyBoard(int source[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            last_board[i][j] = source[i][j];
        }
    }
}

struct square *
findDifferences(int array1[BOARD_WIDTH][BOARD_HEIGHT], int array2[BOARD_WIDTH][BOARD_HEIGHT], int *diffCount) {
    *diffCount = 0;

    int initialSize = 10;
    struct square *differences = (struct square *) malloc(initialSize * sizeof(struct square));

    // Parcourir les deux tableaux et trouver les différences
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (array1[i][j] != array2[i][j]) {
                if (*diffCount >= initialSize) {
                    initialSize *= 2;
                    differences = (struct square *) realloc(differences, initialSize * sizeof(struct square));
                }
                differences[*diffCount].row = i;
                differences[*diffCount].column = j;
                differences[*diffCount].content = array2[i][j];
                (*diffCount)++;
            }
        }
    }

    differences = (struct square *) realloc(differences, (*diffCount) * sizeof(struct square));

    return differences;
}

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    nanosleep(&ts, NULL);
}

void *send_board_changes() {
    sleep(1);

    int compteur = 0;
    while (is_game_over() == -1) {

        int diffCount = 0;

        struct square *differences = findDifferences(last_board, session->board, &diffCount);

        /*for(int i =0; i<diffCount; i++){
            printf("difference col : %d\n", differences[i].column);
            printf("difference row : %d\n", differences[i].row);
            printf("difference content : %d\n", differences[i].content);
        }*/

        struct server_to_client_board_changes board_message_changes = create_server_to_client_board_changes(compteur,
                                                                                                            diffCount,
                                                                                                            differences);

        uint8_t message_buffer[8 + diffCount * 3];

        pack_message_server_board_changes(&board_message_changes, message_buffer);

        /*ssize_t s = sendto(session->sock, message_buffer, sizeof(message_buffer), 0,
                           (struct sockaddr *) &session->adr_multicast, sizeof(session->adr_multicast));
        if (s < 0)
            perror("erreur send\n");*/

        compteur++;

        sleep_ms(200);
    }


    return NULL;
}


void *send_board() {
    int compteur = 0;
    while (is_game_over() == -1) {
        struct server_to_client_board board_message = create_server_to_client_board(compteur);

        fill_grid_data(&board_message, session->board);

        uint8_t message_buffer[2 + 1 + 1 + 2 + 2 + 2 +
                               BOARD_WIDTH * BOARD_HEIGHT];

        pack_message_server_board(&board_message, message_buffer);


        ssize_t s = sendto(session->sock, message_buffer, sizeof(message_buffer), 0,
                           (struct sockaddr *) &session->adr_multicast, sizeof(session->adr_multicast));
        if (s < 0)
            perror("erreur send\n");

        compteur++;

        copyBoard(session->board);


        sleep_ms(200);
        //sleep(1);
    }

    return NULL;
}
