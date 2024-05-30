//
// Created by Samuel on 30/03/2024.
//

#include "game_setup.h"


void create_multicast_adr(GameSession *game, int multicast_port, char *multicast_address) {
    struct sockaddr_in6 adr_multicast;
    memset(&adr_multicast, 0, sizeof(adr_multicast));
    adr_multicast.sin6_family = AF_INET6;
    inet_pton(AF_INET6, multicast_address, &adr_multicast.sin6_addr);
    adr_multicast.sin6_port = htons(multicast_port);
    game->adr_multicast = adr_multicast;
}

int create_game_socket(GameSession *game, int port) {
    int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock < 0) return -1;

    struct sockaddr_in6 adr_receive;
    memset(&adr_receive, 0, sizeof(adr_receive));
    adr_receive.sin6_family = AF_INET6;
    adr_receive.sin6_addr = in6addr_any;
    adr_receive.sin6_port = htons(port);

    int ifindex = if_nametoindex("eth0");
    adr_receive.sin6_scope_id = ifindex; //ou 0 pour l’interface multicast par défaut
    if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifindex, sizeof(ifindex)))
        perror("erreur initialisation de l’interface locale");

    if (bind(sock, (struct sockaddr *) &adr_receive, sizeof(adr_receive)) < 0) return -1;

    game->adr_receive = adr_receive;
    game->sock = sock;

    return 0;
}

void init_player(GameSession *game, const int *client_sock, int mode) {
    game->player[0].sock = client_sock[0];
    game->player[1].sock = client_sock[1];
    game->player[2].sock = client_sock[2];
    game->player[3].sock = client_sock[3];
    //avec 0 1 ensemble et 2 3 ensemble

    for (int i = 0; i < 4; i++) {
        game->player[i].isAlive = 1;
        game->player[i].id = i;
        if (mode == 1) {
            if (i < 2) {
                game->player[i].team = 0;
            } else {
                game->player[i].team = 1;
            }
        } else {
            game->player[i].team = 9;
        }
    }

    game->player[0].x = 1;
    game->player[0].y = 1;
    game->player[3].x = 1;
    game->player[3].y = BOARD_WIDTH - 2;
    game->player[2].x = BOARD_HEIGHT - 2;
    game->player[2].y = 1;
    game->player[1].x = BOARD_HEIGHT - 2;
    game->player[1].y = BOARD_WIDTH - 2;
}

void *game_setup(void *args) {
    struct game_setup_args *setup_args = (struct game_setup_args *) args;

    GameSession game;
    game.mode = setup_args->game_mode;

    init_player(&game, setup_args->sock_copy, setup_args->game_mode);

    for (int i = 0; i < 4; i++) {
        game.bombs[i].x = -1;
        game.bombs[i].y = -1;
        game.bombs[i].active = 0;
    }

    create_board(&game);

    create_multicast_adr(&game, setup_args->multicast_port_copy, setup_args->ip_copy);

    /** creation de la socket de la partie */
    if (create_game_socket(&game, setup_args->udp_port_copy) < 0) {
        perror("Erreur creation socket");
    }

    start_game(&game);

    //close(game.sock);

    return 0;
}

