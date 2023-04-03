
/* Voici une deuxième version en C qui limite le nombre de threads à utiliser à 4 threads.
 * Cette version utilise un pool de threads et une file d'attente pour distribuer le travail aux threads disponibles.
 * Le nombre de threads dans le pool est défini par la constante MAX_THREADS.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_LIGNES 10
#define MAX_COLONNES 10
#define MAX_THREADS 4

// Structure de données partagée entre les threads
typedef struct {
    int l, c;
    int plan[MAX_LIGNES][MAX_COLONNES];
    bool visite[MAX_LIGNES][MAX_COLONNES];
    pthread_mutex_t mutex[MAX_LIGNES][MAX_COLONNES];
    sem_t queue_sem;
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
        return false; // on est tombé sur un mur ou on a déjà visité cette case

    // Verrouillage de la case pour éviter les conflits d'accès
    pthread_mutex_lock(&donnee->mutex[x][y]);
    if (donnee->visite[x][y]) {
        // On a déjà visité cette case pendant qu'on attendait le verrou
        pthread_mutex_unlock(&donnee->mutex[x][y]);
        return false;
    }
    donnee->visite[x][y] = true;  // marquer la case comme visitée
    pthread_mutex_unlock(&donnee->mutex[x][y]);

    // explorer les cases voisines
    if (rechercher(donnee, x+1, y) || rechercher(donnee, x-1, y) || rechercher(donnee, x, y+1) || rechercher(donnee, x, y-1))
        return true;        // on a trouvé un chemin vers la sortie

    return false;
}

void *thread_function(void *arg) {
    argument_thread_t *args = (argument_thread_t*) arg;
    donnee_partage_t *donnee = args->donnee;
    while (true) {
        // attendre un travail dans la file d'attente
        sem_wait(&donnee->queue_sem);
        if (rechercher(donnee, args->x, args->y))
            printf("Un thread  a trouve un chemin vers la sortie\n");
    }
    pthread_exit(NULL);
}

int main() {
    donnee_partage_t donnee;
    pthread_t threads[MAX_THREADS];
    argument_thread_t args[MAX_LIGNES * MAX_COLONNES];
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
            fscanf(fp,"%1d", &donnee.plan[i][j]);
            printf("%d",donnee.plan[i][j]);
            donnee.visite[i][j] = false;
            pthread_mutex_init(&donnee.mutex[i][j], NULL); // initialiser les mutex pour chaque case
        }
        printf("\n");
    }
    pclose(fp);
    // Trouver la position de départ du joueur (case contenant 2)
    for (int i = 0; i < donnee.l; i++)
        for (int j = 0; j < donnee.c; j++)
            if (donnee.plan[i][j] == 2) {
                startX = i;
                startY = j;
                break;
            }


// Initialiser la sémaphore pour la file d'attente
    sem_init(&donnee.queue_sem, 0, 0);

// Lancer les threads
    for (int i = 0; i < MAX_THREADS; i++) {
        args[i].donnee = &donnee;
        args[i].x = startX;
        args[i].y = startY;
        pthread_create(&threads[i], NULL, thread_function, &args[i]);
    }

// Ajouter la position de départ dans la file d'attente
    sem_post(&donnee.queue_sem);

// Attendre la fin du programme (jamais atteint ici car les threads ne se terminent jamais)
    for (int i = 0; i < MAX_THREADS; i++)
        pthread_join(threads[i], NULL);


// Libérer les ressources utilisées
    sem_destroy(&donnee.queue_sem);
    for (int i = 0; i < donnee.l; i++)
        for (int j = 0; j < donnee.c; j++)
            pthread_mutex_destroy(&donnee.mutex[i][j]);


    return 0;}

