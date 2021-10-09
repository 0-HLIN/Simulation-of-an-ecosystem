

#include <stdlib.h>
#include <stdio.h>
#include "ecosys.h"
#include <time.h>


int main() {

    int nb_proie = 6;
    int nb_predateur = 7;
    Animal *l_proie = NULL;
    Animal *l_predateur = NULL;
    Animal *animal1 = NULL;
    Animal *animal2 = NULL;

    srand(time(NULL));


// liste des proies:
    for (int i = 0; i < nb_proie; i++) {
        animal1 = creer_animal( i % SIZE_X, i % SIZE_Y, 2. * i );
        l_proie = ajouter_en_tete_animal(l_proie, animal1);
        //ajouter_animal(i, i, 2.0, &l_proie);
    }

// liste des predateurs:
    animal1 = creer_animal( SIZE_X - 1, SIZE_Y - 1, 1.);
    l_predateur = ajouter_en_tete_animal(l_predateur, animal1);
    for (int i = 1; i < nb_predateur; i++) {
        //animal = creer_animal( SIZE_X / 2, i % SIZE_Y, 2. * i );
        //l_predateur = ajouter_en_tete_animal(l_predateur, animal);
        ajouter_animal( SIZE_X / 2, i, 2.0, &l_predateur);
    }
    animal2 = creer_animal( SIZE_X - 1, 0, 1.);
    l_predateur = ajouter_en_tete_animal(l_predateur, animal2);

// Test
    afficher_ecosys(l_proie, l_predateur);
    printf("Test : compte_animal_rec, compte_animal_it ...\n");
    printf("Nb proies:    %4d\n", compte_animal_rec(l_proie));
    printf("Nb predateurs:%4d\n", compte_animal_it(l_predateur));
    printf("Test : enlever_animal ...\n");
    enlever_animal(&l_predateur, animal1);
    afficher_ecosys(l_proie, l_predateur);
    printf("Test : enlever_animal ...\n");
    enlever_animal(&l_predateur, animal2);
    afficher_ecosys(l_proie, l_predateur);

// Test : bouger_animaux
    printf("Test : bouger_animaux ...\n");
    bouger_animaux(l_proie);
    bouger_animaux(l_predateur);
    afficher_ecosys(l_proie, l_predateur);

    printf("Test : bouger_animaux ...\nAvant:\n");
    animal1 = creer_animal(0, 0, 1);
    afficher_ecosys(animal1, NULL);
    animal1->dir[0] = 1;
    animal1->dir[1] = 1;
    bouger_animaux(animal1);
    printf("Apres:\n");
    afficher_ecosys(animal1, NULL);
    printf("Test : reproduce ...\n");
    reproduce(&animal1, 1.0);
    printf("Nb animaux : %4d\n", compte_animal_it(animal1));
    printf("Test : reproduce ...\n");
    reproduce(&animal1, 1.0);
    printf("Nb animaux : %4d\n", compte_animal_it(animal1));

    liberer_liste_animaux(animal1);


// Liberation de la memoire
    liberer_liste_animaux(l_proie);
    liberer_liste_animaux(l_predateur);


    return 0;
}


