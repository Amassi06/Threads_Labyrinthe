/*
 Ce code est un programme en C qui définit deux constantes MAX_LIGNES et MAX_COLONNES,
 et utilise la fonction system() pour appeler un script Python appelé "generateur.py"
 avec ces deux constantes comme arguments.

 La commande à exécuter est stockée dans la variable command à l'aide de la fonction sprintf().
 et affiche le labyrinthe dans le terminal.

     0 --> mur
     1 --> route
     2 --> position de départ
     3 --> position de sortie
 
 Le programme se termine en retournant la valeur 0.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAX_LIGNES  10
#define MAX_COLONNES 10

int main() {

    char command[100];

    sprintf(command, "python generateur.py %d %d", MAX_LIGNES, MAX_COLONNES);
    system(command);

    return 0;
}
