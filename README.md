# Threads_Labyrinthe

# Contexte
Le but de ce projet est d'utiliser vos compétences en C et vos connaissances des threads et des processus pour résoudre efficacement un problème de résolution de labyrinthe. L'idée est d'écrire une première version séquentielle d'un algorithme de résolution de labyrinthe. Puis, l'objectif sera de paralléliser la résolution de l'algorithme en utilisant des threads. Enfin, un dernier axe bonus sera de résoudre une instance de labyrinthe à deux entrées et deux sorties. Dans ce dernier cas, deux joueurs devront résoudre en simultané un labyrinthe. La différence avec la résolution à une entrée est que pour arriver à résoudre une instance à deux entrées, il faudra synchroniser les joueurs pour qu'ils puissent passer une porte.

# Consignes
L'objectif est de résoudre une instance de labyrinthe.

->écrire un programme qui génère une instance de labyrinthe simple (à une entrée et une sortie).
->écrire un premier programme qui résolve une instance de labyrinthe simple.
->écrire une deuxième méthode de résolution de labyrinthe utilisant des threads pour la résolution de labyrinthe.
    -écrire une première version sans limite du nombre de threads
    -écrire une deuxième version qui devra limiter le nombre de threads à utiliser
    
# Schéma

     0 --> mur
     1 --> route
     2 --> position de départ
     3 --> position de sortie
   
   
   
[!alt][https://github.com/Amassi06/Threads_Labyrinthe/blob/main/Capture%20d'%C3%A9cran%202023-04-03%20192331.png]
