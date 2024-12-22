#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <adresse_ip> <port>\n", argv[0]);
        return 1;
    }

    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == -1) {
        perror("Erreur de création du socket TCP");
        return 1;
    }

    struct sockaddr_in addrServer;
    memset(&addrServer, 0, sizeof(addrServer));
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(atoi(argv[2]));
    addrServer.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(socketClient, (struct sockaddr *)&addrServer, sizeof(addrServer)) == -1) {
        perror("Erreur de connexion au serveur");
        return 0;
    }
    printf("Connexion au serveur réussie\n");

    while (1) {
        // Réception du compteur depuis le serveur
        if (recv(socketClient, &compteur, sizeof(int), 0) <= 0) {
            perror("Erreur de réception des données");
            break;
        }
        printf("[Client] Réception : %d\n", compteur);

        // Incrémentation et envoi au serveur
        compteur++;
        sleep(1);
        if (send(socketClient, &compteur, sizeof(int), 0) == -1) {
            perror("Erreur d'envoi des données");
            break;
        }
        printf("[Client] Envoi : %d\n", compteur);

    }

    close(socketClient);
    return 0;
}
