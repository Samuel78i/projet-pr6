//
// Created by Samuel on 30/03/2024.
//

#include "connexion_serv.h"

struct sock_and_adr *connexion(int port) {
    struct sock_and_adr *sock_and_adr = (struct sock_and_adr *) malloc(sizeof(struct sock_and_adr));
    int sock = socket(PF_INET6, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Creating socket");
        exit(1);
    }

    /** Creation de l’adresse du destinataire (serveur) */
    struct sockaddr_in6 adr;
    memset(&adr, 0, sizeof(adr));
    adr.sin6_family = AF_INET6;
    adr.sin6_port = htons(port);
    adr.sin6_addr = in6addr_any;

    if (bind(sock, (struct sockaddr *) &adr, sizeof(adr)) < 0) {
        perror("Error while binding socket");
        return NULL;
    }

    int l = listen(sock, 0);
    if (l < 0) { exit(1); } else { printf("j'écoute\n"); }

    sock_and_adr->sock = sock;
    sock_and_adr->adr = adr;
    return sock_and_adr;
}
