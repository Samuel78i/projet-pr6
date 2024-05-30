//
// Created by Samuel on 30/03/2024.
//

#include "in_game.h"

board client_board;
line client_line;

void convertGridToChar(const uint8_t grid[], char *board_grid, int length) {
    for (int i = 0; i < length - 1; i++) {
        board_grid[i] = (char) grid[i];
    }
}

int get_grid(board *b, int x, int y) {
    return b->grid[y * b->w + x];
}


void refresh_game(board *b, line *l) {
    int x, y;

    mvaddch(0, 0, ACS_ULCORNER);
    for (x = 1; x <= b->w * 2; x++) {
        mvaddch(0, x, ACS_HLINE);
    }
    mvaddch(0, b->w * 2 + 1, ACS_URCORNER);

    mvaddch(b->h + 1, 0, ACS_LLCORNER);
    for (x = 1; x <= b->w * 2; x++) {
        mvaddch(b->h + 1, x, ACS_HLINE);
    }
    mvaddch(b->h + 1, b->w * 2 + 1, ACS_LRCORNER);

    for (y = 1; y <= b->h; y++) {
        mvaddch(y, 0, ACS_VLINE);
        mvaddch(y, b->w * 2 + 1, ACS_VLINE);
    }

    for (y = 0; y < b->w; y++) {
        for (x = 0; x < b->h; x++) {
            char c;
            switch (get_grid(b, x, y)) {
                case 0:
                    c = '0';
                    break;
                case 1:
                    c = '1';
                    break;
                case 2:
                    c = '2';
                    break;
                case 3:
                    c = '3';
                    break;
                case 5:
                    c = '5';
                    break;
                case 6:
                    c = '6';
                    break;
                case 7:
                    c = '7';
                    break;
                case 8:
                    c = '8';
                    break;
                default:
                    c = '?';
                    break;
            }
            mvaddch(y + 1, x * 2 + 1, c);
            mvaddch(y + 1, x * 2 + 2, ' ');
        }
    }

    attron(COLOR_PAIR(1));
    attron(A_BOLD);
    for (x = 0; x < b->w * 2 + 2; x++) {
        if (x / 2 >= TEXT_SIZE || x / 2 >= l->cursor) {
            mvaddch(b->h + 2, x, ' ');
        } else {
            mvaddch(b->h + 2, x, l->data[x / 2]);
        }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));

    refresh();
}

void update_board_changes(int nb_changes, struct square *changes, board *b, line *l) {
    for (int i = 0; i < nb_changes; i++) {
        int x = changes[i].row;
        int y = changes[i].column;

        b->grid[y * b->h + x] = (char) changes[i].content;
    }
    refresh_game(b, l);
}


_Noreturn void *send_action(void *arg) {
    struct game_data data;
    data = *(struct game_data *) arg;
    int key_pressed;
    int count_mess = 0;

    while (1) {
        uint8_t action = 9;
        key_pressed = getch();

        if (key_pressed == 'z') {
            action = 0;
        }
        if (key_pressed == 'd') {
            action = 1;
        }
        if (key_pressed == 's') {
            action = 2;
        }
        if (key_pressed == 'q') {
            action = 3;
        }
        if (key_pressed == 'b') {
            action = 4;
        }
        if (key_pressed == 'c') {
            action = 5;
        }
        //pour le tchat
        if (key_pressed == 't') {
            send_message_chat(data.sock_tcp, data.eq, data.id);
        } else {
            if (action != 9) {
                struct message_client_played client_udp_message = create_message_client_udp(data.code, data.id, data.eq,
                                                                                            count_mess, action);
                uint8_t message_buffer[7];
                pack_message_client_played(&client_udp_message, message_buffer);

                ssize_t s = sendto(data.sock, message_buffer, sizeof(message_buffer), 0, (struct sockaddr *) &data.adr,
                                   sizeof(data.adr));
                if (s < 0)
                    perror("erreur send\n");

                count_mess++;
            }
        }
    }
}

void *recv_mess_chat(void *arg) {
    // recevoir message server_to_client_chat
    // soit codereq = 13 ou 14 ou 15 ou 16
    // 15 ou 16 ferme tous les socket et exit() pour quiter
    // sinon 13 ou 14 modifier line pour afficher data
    int sock = *(int *) arg;

    /*fd_set readfds;
    while (1) {
        FD_ZERO(&readfds);

        FD_SET(sock, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
        if (activity > 1) {
            if (FD_ISSET(sock, &readfds)) {

                uint8_t bufrecv[1024];

                if (read(sock, bufrecv, sizeof(struct message_fin)) < 0) {
                    perror("erreur read");
                }
                uint16_t codereq = ntohs(*((uint16_t * )(bufrecv + 0)));

                if (codereq == 13) {
                    struct message_client_chat messageClientChat;
                    unpack_message_chat(bufrecv, &messageClientChat);
                    strcpy(client_line.data, messageClientChat.data);
                    client_line.cursor = messageClientChat.len;
                }else {
                    endwin();
                    struct message_fin messageFin;
                    unpack_message_fin(bufrecv, &messageFin);
                    if(codereq == 15){
                        printf("the winner is %d", messageFin.id);
                    }else{
                        printf("winner team is %d", messageFin.eq);
                    }
                    close(sock);
                    exit(0);
                }

            }
        }
    }*/

    uint8_t bufrecv[1024];

    if (read(sock, bufrecv, sizeof(struct message_fin)) < 0) {
        perror("erreur read");
    }
    uint16_t codereq = ntohs(*((uint16_t * )(bufrecv + 0)));

    if (codereq == 13) {
        struct message_client_chat messageClientChat;
        unpack_message_chat(bufrecv, &messageClientChat);
        strcpy(client_line.data, messageClientChat.data);
        client_line.cursor = messageClientChat.len;
    } else {
        endwin();
        struct message_fin messageFin;
        unpack_message_fin(bufrecv, &messageFin);
        if (codereq == 15) {
            printf("the winner is %d\n", messageFin.id);
        } else {
            printf("winner team is %d\n", messageFin.eq);
        }
        close(sock);
        exit(0);
    }
    return NULL;
}

void unpack_find_which_board(const uint8_t *buffer) {
    uint16_t codereq = ntohs(*((uint16_t * )(buffer + 0)));


    if (codereq == 12) {
        struct server_to_client_board_changes server_to_client_board_changes;
        unpack_message_server_board_changes(buffer, &server_to_client_board_changes);

        if (server_to_client_board_changes.nb > 0) {
            update_board_changes(server_to_client_board_changes.nb, server_to_client_board_changes.square,
                                 &client_board, &client_line);
        }
    } else {
        struct server_to_client_board board_mdiff_client;
        unpack_message_server_board(buffer, &board_mdiff_client);
        client_board.h = (int) board_mdiff_client.height;
        client_board.w = (int) board_mdiff_client.width;
        int len = (int) board_mdiff_client.height * (int) board_mdiff_client.width;

        char temp[len];
        convertGridToChar(board_mdiff_client.grid, temp, len);
        // convertir uint8_t grid[BOARD_WIDTH * BOARD_HEIGHT] à char *
        client_board.grid = temp;

        refresh_game(&client_board, &client_line);
    }
}


_Noreturn void *recv_mess(void *arg) {
    // recevoir message server_to_client_board par seconde et
    // à partir le board de cette message,changer le grid dans client_board
    //  à le grid represente dans message
    // reaficher le board
    int sock = *(int *) arg;
    while (1) {

        uint8_t bufrecv[1024];

        if (read(sock, bufrecv, sizeof(struct server_to_client_board)) < 0) {
            perror("erreur read");
        } else {
            unpack_find_which_board(bufrecv);
        }
    }

}

void game_start(int sock_serv, int sock_udp, struct sockaddr_in6 adr_serv, uint8_t id, uint8_t codereq,
                uint8_t team_number) {
    pthread_t thread[3];
    struct game_data data;
    data.sock = sock_udp;
    data.adr = adr_serv;
    data.code = codereq;
    data.id = id;
    data.eq = team_number;
    // sock en tcp
    data.sock_tcp = sock_serv;
    // avant deroulement du jeu
    //  ncurse init

    initscr();
    cbreak();
    noecho();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    // et quand server envoie grid, commence à afficher
    // ici pas depande à l'entré de clavier
    // mais le message reçu et envoyé
    pthread_create(&thread[0], NULL, send_action, &data);
    pthread_create(&thread[1], NULL, recv_mess, &sock_udp);
    pthread_create(&thread[2], NULL, recv_mess_chat, &sock_serv);
    int *ptr;

    // wait for threads
    pthread_join(thread[0], (void **) &ptr);
    pthread_join(thread[1], (void **) &ptr);
    pthread_join(thread[2], (void **) &ptr);

    close(sock_udp);
}