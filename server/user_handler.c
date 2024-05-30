//
// Created by Samuel on 30/03/2024.
//

#include "user_handler.h"

GameSession gameSessions[10]; // variable pour instant pour les sessions du jeu , possible de changer dans un autre fichier plus appropriee.

struct users {
    int user_count;
    int sock[4];
    int ready;
};

struct users solo;
struct users team;

int udp_port_solo = 12001;
int udp_port_team = 12000;

int multicast_port_solo = 10001;
int multicast_port_team = 10000;

char *ip_solo = NULL;
char *ip_team = NULL;


char *generate_random_ip() {
    static uint32_t counter = 1; // Starting from 1
    static char ip[40];  // IPv6 addresses can be up to 39 characters long + 1 for the null terminator

    // Generate the IP address based on the counter
    snprintf(ip, sizeof(ip), "ff12::%x", counter);

    // Increment the counter for the next call
    counter++;

    return ip;
}

int sockNotInArray(int client_sock, const int *sock_array) {
    for (int i = 0; i < 4; i++) {
        if (sock_array[i] == client_sock) {
            return 0;
        }
    }
    return 1;
}


/** Je voudrais gérer ici l'attente des parties */
int add_user(int game_sock, int client_sock, uint8_t *messageClientToJoin_buffer) {
    struct message_client_to_join messageClientToJoin;
    unpack_message_ctj(messageClientToJoin_buffer, &messageClientToJoin);

    if (ip_solo == NULL) {
        ip_solo = generate_random_ip();
    }
    if (ip_team == NULL) {
        ip_team = generate_random_ip();
    }

    if (messageClientToJoin.codereq == 1 && solo.ready != 4 && sockNotInArray(client_sock, solo.sock)) {
        solo.sock[solo.user_count] = client_sock;
        solo.user_count++;

        struct server_to_client_create serv_message =
                create_server_to_client_create(9, solo.user_count, solo.user_count % 2, udp_port_solo,
                                               multicast_port_solo, ip_solo);

        // Création d'un tableau d'octets pour stocker le message
        uint8_t message_buffer[7 + strlen(ip_solo)];

        pack_message_stcc(&serv_message, message_buffer);

        send(client_sock, message_buffer, sizeof(message_buffer), 0);
    } else if (messageClientToJoin.codereq == 2 && team.ready != 4 && sockNotInArray(client_sock, solo.sock)) {
        team.sock[team.user_count] = client_sock;
        team.user_count++;

        struct server_to_client_create serv_message =
                create_server_to_client_create(10, team.user_count, team.user_count % 2, udp_port_team,
                                               multicast_port_team, ip_team);

        // Création d'un tableau d'octets pour stocker le message
        uint8_t message_buffer[25]; // 1 + 1 + 1 + 2 + 2 + 16 = 25

        pack_message_stcc(&serv_message, message_buffer);

        send(client_sock, message_buffer, sizeof(message_buffer), 0);
    }

    if (messageClientToJoin.codereq == 3 || messageClientToJoin.codereq == 4) {
        for (int i = 0; i < 4; i++) {
            if (solo.sock[i] == client_sock) {
                solo.ready++;
            }
            if (team.sock[i] == client_sock) {
                team.ready++;
            }
        }
    }

    if (solo.user_count == 4 && solo.ready == 4) {
        struct game_setup_args *solo_args = malloc(sizeof(struct game_setup_args));
        if (solo_args == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        memcpy(solo_args->sock_copy, solo.sock, sizeof(solo.sock));
        solo_args->game_sock = game_sock;
        solo_args->udp_port_copy = udp_port_solo;
        solo_args->multicast_port_copy = multicast_port_solo;
        solo_args->ip_copy = strdup(ip_solo);
        solo_args->game_mode = 0;

        solo.user_count = 0;
        solo.ready = 0;
        for (int i = 0; i < 4; i++) {
            solo.sock[i] = -1;
        }
        ip_solo = NULL;

        udp_port_solo++;
        multicast_port_solo++;

        pthread_t solo_thread;
        if (pthread_create(&solo_thread, NULL, game_setup, (void *) solo_args) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        pthread_detach(solo_thread);

    }
    if (team.user_count == 4 && team.ready == 4) {
        struct game_setup_args *team_args = malloc(sizeof(struct game_setup_args));
        if (team_args == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        memcpy(team_args->sock_copy, team.sock, sizeof(team.sock));
        team_args->game_sock = game_sock;
        team_args->udp_port_copy = udp_port_team;
        team_args->multicast_port_copy = multicast_port_team;
        team_args->ip_copy = strdup(ip_team);
        team_args->game_mode = 1;

        team.user_count = 0;
        team.ready = 0;
        for (int i = 0; i < 4; i++) {
            team.sock[i] = -1;
        }
        ip_team = NULL;

        udp_port_team--;
        multicast_port_team--;

        pthread_t team_thread;
        if (pthread_create(&team_thread, NULL, game_setup, (void *) team_args) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        pthread_detach(team_thread);
    }
    return 0;
}

int remove_user(int sock) {
    // TODO
    return 0;
}
