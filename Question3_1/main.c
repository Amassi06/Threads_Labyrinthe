#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_LIGNES 10
#define MAX_COLONNES 10

// Structure de données partagée entre les threads
typedef struct {
    int l, c;
    int plan[MAX_LIGNES][MAX_COLONNES];
    bool visite[MAX_LIGNES][MAX_COLONNES];
    pthread_mutex_t mutex[MAX_LIGNES][MAX_COLONNES];
} donnee_partage_t;

// Structure pour les arguments passés aux threads
typedef struct {
    donnee_partage_t *donnee;
    int x, y;
} argument_thread_t;


bool rechercher(donnee_partage_t *donnee, int x, int y) {

    if (x < 0 || x >= donnee->l || y < 0 || y >= donnee->c)
        return false;        // on a atteint le bord du labyrinthe


    if (donnee->plan[x][y] == 3)
        return true;        // on a trouvé la sortie


    if (donnee->plan[x][y] == 0 || donnee->visite[x][y])
        return false;        // on est tombé sur un mur ou on a déjà visité cette case


    // Verrouillage de la case pour éviter les conflits d'accès
    pthread_mutex_lock(&donnee->mutex[x][y]);
    if (donnee->visite[x][y]) {
        pthread_mutex_unlock(&donnee->mutex[x][y]); // On a déjà visité cette case pendant qu'on attendait le verrou
        return false;
    }
    donnee->visite[x][y] = true;  // marquer la case comme visitée
    pthread_mutex_unlock(&donnee->mutex[x][y]);

    // explorer les cases voisines
    if (rechercher(donnee, x + 1, y) || rechercher(donnee, x - 1, y) || rechercher(donnee, x, y + 1) ||
        rechercher(donnee, x, y - 1))
        return true;        // on a trouvé un chemin vers la sortie

    return false;
}

void *thread_function(void *arg) {
    argument_thread_t *args = (argument_thread_t *) arg;
    rechercher(args->donnee, args->x, args->y);
    pthread_exit(NULL);
}

int main() {
    donnee_partage_t donnee;
    pthread_t threads[MAX_LIGNES][MAX_COLONNES];
    donnee.l = MAX_LIGNES;
    donnee.c = MAX_COLONNES;
    int startX, startY;

    // Exécuter la commande pour générer le labyrinthe et lire la sortie avec popen()
    char command[100];
    sprintf(command, "python generateur.py %d %d", MAX_LIGNES, MAX_COLONNES);
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("Erreur lors de l'exécution de la commande");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < donnee.l; i++) {
        for (int j = 0; j < donnee.c; j++) {
            fscanf(fp, "%1d", &donnee.plan[i][j]);
            printf("%d", donnee.plan[i][j]);
            donnee.visite[i][j] = false;
            pthread_mutex_init(&donnee.mutex[i][j], NULL);
        }
        printf("\n");
    }
    pclose(fp);

    // recherche d'un chemin entre la position de départ et la position de sortie
    for (int i = 0; i < donnee.l; i++)
        for (int j = 0; j < donnee.c; j++)
            if (donnee.plan[i][j] == 2) {
                startX = i;
                startY = j;
                break;
            }



// Lancer des threads pour chercher un chemin à partir de chaque case non-murale
    for (int i = startX; i < donnee.l; i++)
        for (int j = startY; j < donnee.c; j++)
            if (donnee.plan[i][j] != 0) {
                argument_thread_t args = {&donnee, i, j};
                pthread_create(&threads[i][j], NULL, thread_function, (void *) &args);
            }


    // Attendre que tous les threads aient fini
    for (int i = 0; i < donnee.l; i++)
        for (int j = 0; j < donnee.c; j++)
            if (donnee.plan[i][j] != 0)
                pthread_join(threads[i][j], NULL);

    // Afficher le résultat
    if (donnee.visite[startX][startY])
        printf("Il existe un chemin vers la sortie.\n");
    else
        printf("Il n'existe pas de chemin vers la sortie.\n");

    // Détruire les verrous
    for (int i = 0; i < donnee.l; i++)
        for (int j = 0; j < donnee.c; j++)
            pthread_mutex_destroy(&donnee.mutex[i][j]);

    return 0;
}