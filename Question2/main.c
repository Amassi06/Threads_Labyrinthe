/*
 * Ce code en langage C est utilisé pour rechercher un chemin à travers un labyrinthe généré aléatoirement
 * en Python.Il utilise deux tableaux plan et visite pour stocker le labyrinthe et les cases visitées,respectivement.
 * La fonction rechercher() est récursive et est utilisée pour explorer les cases voisines afin de trouver
 * un chemin vers la sortie. Le programme commence par exécuter une commande en Python pour générer le
 * labyrinthe et lire sa sortie en utilisant la fonction popen().
 * Il parcourt ensuite le labyrinthe pour trouver la position de départ et appelle la fonction rechercher()
 * pour trouver un chemin vers la sortie.Le programme affiche enfin si un chemin a été trouvé ou non.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LIGNES 10
#define MAX_COLONNES 10

int plan[MAX_LIGNES][MAX_COLONNES];
bool visite[MAX_LIGNES][MAX_COLONNES];

bool rechercher(int x, int y) {
    if (x < 0 || x >= MAX_LIGNES || y < 0 || y >= MAX_COLONNES)
        return false;// on a atteint le bord du labyrinthe

    if (plan[x][y] == 3)
        return true;// on a trouvé la sortie

    if (plan[x][y] == 0 || visite[x][y])
        return false; // on est tombé sur un mur ou on a déjà visité cette case


    visite[x][y] = true;  // marquer la case comme visitée
    // explorer les cases voisines
    if (rechercher(x + 1, y) || rechercher(x - 1, y) || rechercher(x, y + 1) || rechercher(x, y - 1))
        return true;// on a trouvé un chemin vers la sortie

    return false;
}

int main() {
    // Exécuter la commande pour générer le labyrinthe et lire la sortie avec popen()
    char command[100];
    sprintf(command, "python generateur.py %d %d", MAX_LIGNES, MAX_COLONNES);
    FILE *fp = popen(command, "r");
    if (!fp) {
        printf("Erreur lors de l'exécution de la commande");
        exit(EXIT_FAILURE);
    }

    // lecture du labyrinthe depuis la sortie de la commande
    for (int i = 0; i < MAX_LIGNES; i++) {
        for (int j = 0; j < MAX_COLONNES; j++) {
            fscanf(fp, "%1d", &plan[i][j]); //nous avons utilisé "%1d" dans fscanf pour lire un seul chiffre à la fois,
            visite[i][j] = false;
            printf("%d", plan[i][j]);
        }
        printf("\n");
    }
    pclose(fp);


    // recherche d'un chemin entre la position de départ et la position de sortie
    int startX, startY;
    for (int i = 0; i < MAX_LIGNES; i++)
        for (int j = 0; j < MAX_COLONNES; j++)
            if (plan[i][j] == 2) {
                startX = i;
                startY = j;
                break;
            }

    if (rechercher(startX, startY))
        printf("Il y a un chemin entre la position de depart et la position de sortie.\n");
    else
        printf("Il n'y a pas de chemin entre la position de depart et la position de sortie.\n");

    return 0;
}