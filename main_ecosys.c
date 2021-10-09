
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "ecosys.h"
#include <unistd.h>
#include <assert.h>

#define NB_PROIES     20
#define NB_PREDATEURS 10
#define NRJ_PROIE     10
#define NRJ_PREDATEUR 20
#define MAX_ITER     500
#define T_WAIT   100000

/* Parametres gloabaux de l'ecosysteme (externe dans le ecosys.h) */


int main() {
    int monde[SIZE_X][SIZE_Y];
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    FILE *pf = NULL;

    srand(time(NULL));

// Initialisation : monde
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            monde[i][j] = 0;
        }
    }
// Initialisation : liste_proie
    for (int i = 0; i < NB_PROIES; i++) {
        ajouter_animal( rand()%SIZE_X, rand()%SIZE_Y, NRJ_PROIE, &liste_proie);
    }
// Initialisation : liste_predateur
    for (int i = 0; i < NB_PREDATEURS; i++) {
        ajouter_animal( rand()%SIZE_X, rand()%SIZE_Y, NRJ_PREDATEUR, &liste_predateur);
    }

// Ouverture du fichier
    pf = fopen("Evol_Pop.txt", "w");
    assert( pf != NULL );

// Evolution du ecosysteme
    clear_screen();
    afficher_ecosys(liste_proie, liste_predateur);
    for (int i = 0; i < MAX_ITER; i++) {
        if ( compte_animal_it(liste_proie) <= 0 && compte_animal_it(liste_predateur) <= 0 ) break;

        /* ecriture dans le fichier */
        fprintf(pf, "%d %d %d\n", i, compte_animal_it(liste_proie), compte_animal_it(liste_predateur) );

        /* rafraichissement du ecosysteme */
        rafraichir_monde( monde );
        rafraichir_proies( &liste_proie, monde );
        rafraichir_predateurs( &liste_predateur, &liste_proie );

        /* rafraichissement de l'affichage */
        usleep(T_WAIT);        
        system("clear");    /* effacer l'ecran */
        //clear_screen();    /* ne semble pas marcher */
        printf("Iteration  %4d / %4d:\n", i+1, MAX_ITER);
        afficher_ecosys(liste_proie, liste_predateur);
    }

// Fermeture du fichier et dernier ecriture
    fprintf(pf, "%d %d %d\n", MAX_ITER, compte_animal_it(liste_proie), compte_animal_it(liste_predateur) );
    fclose(pf);

// liberation de la memoire
    liberer_liste_animaux(liste_predateur);
    liberer_liste_animaux(liste_proie);

    return 0;
}
