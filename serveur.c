#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int socketServerTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServerTCP == -1) {
        perror("Erreur de création du socket TCP");
        return 1;
    }
    printf("Création du socket TCP réussie\n");

    struct sockaddr_in addrServer;
    memset(&addrServer, 0, sizeof(addrServer));
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(atoi(argv[1]));
    // addrServer.sin_addr.s_addr = INADDR_ANY;
    addrServer.sin_addr.s_addr=inet_addr("0.0.0.0");

    if (bind(socketServerTCP, (struct sockaddr *)&addrServer, sizeof(addrServer)) == -1) {
        perror("Erreur de lancement du bind");
        return 0;
    }
    printf("Bind réussi\n");

    if (listen(socketServerTCP, 10) == -1) {
        perror("Erreur de lancement de listen");
        return 0;
    }
    printf("En attente de connexions...\n");

    struct sockaddr_in addrClient;
    int taille = sizeof(addrClient);

    int socketClientTCP = accept(socketServerTCP, (struct sockaddr *)&addrClient, &taille);
    if (socketClientTCP == -1) {
        perror("Erreur d'acceptation de connexion");
        close(socketServerTCP);
        return 1;
    }
    printf("Connexion acceptée pour le client : %s  :  %d\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));

    compteur=0;
    while (1) {
        // Envoi du compteur au client
        if (send(socketClientTCP, &compteur, sizeof(int), 0) == -1) {
            perror("Erreur d'envoi des données");
            break;
        }
        printf("[Serveur] Envoi : %d\n", compteur);

        // Réception du compteur depuis le client
        if (recv(socketClientTCP, &compteur, sizeof(int), 0) <= 0) {
            perror("Erreur de réception des données");
            break;
        }
        printf("[Serveur] Réception : %d\n", compteur);

        // Incrémentation et pause
        compteur++;
        sleep(1);
    }

    close(socketClientTCP);
    close(socketServerTCP);
    return 0;
}
