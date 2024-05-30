//
// Created by Samuel on 30/03/2024.
//

#include "main_serv.h"


int count = 0;

void *new_client(void *arg) {
    struct thre *t = (struct thre *) arg;

    while (1) {

        struct sockaddr_in6 addrclient;
        socklen_t size = sizeof(addrclient);

        t->sock_client[count] = accept(t->sockAndAdr->sock, (struct sockaddr *) &addrclient, &size);


        uint8_t messageClientToJoin_buffer[SIZE_MSG + 1];

        if (read(t->sock_client[count], messageClientToJoin_buffer, sizeof(messageClientToJoin_buffer)) < 0) {
            perror("error read");
        }

        add_user(t->sockAndAdr->sock, t->sock_client[count], messageClientToJoin_buffer);

        count++;
    }
}

void *ready_client(void *arg) {
    struct thre *t = (struct thre *) arg;
    fd_set readfds;
    uint8_t messageClientToJoin_buffer[SIZE_MSG + 1];
    int max_sd, sd, activity, valread;

    while (1) {
        FD_ZERO(&readfds);
        max_sd = 0;

        for (int i = 0; i < count; i++) {
            sd = t->sock_client[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }

        for (int i = 0; i < count; i++) {
            sd = t->sock_client[i];

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, messageClientToJoin_buffer, SIZE_MSG)) > 0) {
                    add_user(t->sockAndAdr->sock, t->sock_client[i], messageClientToJoin_buffer);
                }
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = 7777;//TCP port
    if (argc > 1) {
        strtol(argv[1], (char **) "%i", port);
    }

    struct sock_and_adr *sockAndAdr = connexion(port);


    struct thre t;
    t.sockAndAdr = sockAndAdr;
    memset(t.sock_client, 0, sizeof(t.sock_client));

    pthread_t thread[2];

    pthread_create(&thread[0], NULL, new_client, &t);
    pthread_create(&thread[1], NULL, ready_client, &t);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    close(t.sockAndAdr->sock);
    return 0;
}
