//
// Created by Samuel on 30/03/2024.
//

#include "join_game.h"

void sent_ready(int sock_serv, int mode) {
    struct message_client_to_join message_ctj = create_message_client_to_join(mode);
    uint8_t message_buffer[4];
    pack_message_ctj(&message_ctj, message_buffer);
    ssize_t s = send(sock_serv, message_buffer, sizeof(message_buffer), 0);
    if (s < 0)
        perror("error send\n");

    /*char buf_recv2[SIZE_MSG + 1];
    if (read(sock_serv, buf_recv2, SIZE_MSG) < 0) {
        perror("error read");
    }*/
}

int setup_udp_sock(int port) {

    struct sockaddr_in6 adrcli;
    memset(&adrcli, 0, sizeof(adrcli));
    adrcli.sin6_family = AF_INET6;
    adrcli.sin6_addr = in6addr_any;
    adrcli.sin6_port = htons(port);

    int sock = socket(AF_INET6, SOCK_DGRAM, 0);

    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof reuse) < 0) {
        perror("Setting SO_REUSEADDR error");
        close(sock);
        return 1;
    }

    if (bind(sock, (struct sockaddr *) &adrcli, sizeof(adrcli))) {
        perror("erreur bind");
        close(sock);
    }
    return sock;
}

void subscribe(int sock, char *ip) {
    struct ipv6_mreq group;
    inet_pton(AF_INET6, ip, &group.ipv6mr_multiaddr.s6_addr);
    group.ipv6mr_interface = if_nametoindex("eth0");
    if (setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, &group, sizeof(group)) < 0) {
        perror("error subscribe");
        close(sock);
    }
}

int handle_server_response(int sock_tcp, int mode) {
    uint8_t received_buffer[SIZE_MSG + 1];
    if (read(sock_tcp, received_buffer, sizeof(received_buffer)) < 0) {
        perror("error read");
    }
    struct server_to_client_create message_stcc;

    unpack_message_stcc(received_buffer, &message_stcc);

    int sock_udp = setup_udp_sock(message_stcc.multicast_port);

    subscribe(sock_udp, message_stcc.multicast_address);

    sent_ready(sock_tcp, mode);
    struct sockaddr_in6 adr;
    memset(&adr, 0, sizeof(adr));
    adr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, IP_SERV, &adr.sin6_addr);
    adr.sin6_port = htons(message_stcc.udp_port);

    game_start(sock_tcp, sock_udp, adr, message_stcc.id, message_stcc.codereq, message_stcc.team_number);
    return 0;
}

int join_team_game(int sock) {
    struct message_client_to_join message_ctj = create_message_client_to_join(2);
    uint8_t message_buffer[4];
    pack_message_ctj(&message_ctj, message_buffer);

    ssize_t s = send(sock, message_buffer, sizeof(message_buffer), 0);
    if (s < 0)
        perror("error send\n");

    handle_server_response(sock, 4);

    return 0;
}

int join_solo_game(int sock) {

    struct message_client_to_join message_ctj = create_message_client_to_join(1);
    uint8_t message_buffer[4];
    pack_message_ctj(&message_ctj, message_buffer);

    ssize_t s = send(sock, message_buffer, sizeof(message_buffer), 0);
    if (s < 0)
        perror("error send\n");

    handle_server_response(sock, 3);

    return 0;
}