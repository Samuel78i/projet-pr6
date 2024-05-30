//
// Created by Samuel on 30/03/2024.
//

#include "connexion_cli.h"

int connexion(char *ip, int port) {
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Error while creating the socket");
        return -1;
    }

    struct sockaddr_in6 adrso;
    memset(&adrso, 0, sizeof(adrso));
    adrso.sin6_family = AF_INET6;
    adrso.sin6_port = htons(port);
    inet_pton(AF_INET6, ip, &adrso.sin6_addr);

    int r = connect(sock, (struct sockaddr *) &adrso, sizeof(adrso));
    if (r == -1) {
        perror("Error while connecting");
        close(sock);
        return -1;
    }

    return sock;
}