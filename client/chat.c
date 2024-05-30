//
// Created by Samuel on 30/03/2024.
//

#include "chat.h"

void send_message_chat(int sock, uint16_t eq, uint8_t id) {
    printf("message send to co-equipe or tous (1/0)\n");

    char answer[10];
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';

    char text[255];
    printf("Entrez le message : \n");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    struct message_client_chat mss;
    mss.eq = eq;
    mss.id = id;
    mss.len = strlen(text);
    strcpy(mss.data, text);

    if (strcmp(answer, "1") == 0) {
        mss.codereq = 8;
    } else if (strcmp(answer, "0") == 0) {
        mss.codereq = 7;
    }

    size_t total = 0;
    size_t mss_size = sizeof(mss);
    while (total < mss_size) {
        ssize_t n = send(sock, ((char *) &mss) + total, mss_size - total, 0);
        if (n == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
        total += n;
    }
}

