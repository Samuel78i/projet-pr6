//
// Created by Samuel on 30/03/2024.
//

#include "main_cli.h"


int main(int argc, char *argv[]) {
    int port = 7777;

    if (argc > 1) {
        strtol(argv[1], (char **) "%i", port);
    }

    int sock = connexion(IP_SERV, port);
    if (sock == -1) {
        return -1;
    }

    printf("Connected to server\n");
    printf("Would you like to play in the team game mode? (yes/no)\n");

    join_solo_game(sock);
    /*char answer[6];
    scanf("%s", answer);

    if (strcmp(answer, "yes") == 0) {
        printf("Joining team game\n");
        join_team_game(sock);
    } else if (strcmp(answer, "no") == 0) {
        printf("Joining solo game\n");
        join_solo_game(sock);
    }*/
}
