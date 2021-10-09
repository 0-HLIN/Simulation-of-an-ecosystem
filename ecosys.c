
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "ecosys.h"


/* Parametres gloabaux de l'ecosysteme (externe dans le ecosys.h) */
float p_ch_dir = 0.05;
int gain_energie_proie = 14;
int gain_energie_predateur = 20;
float p_reproduce_proie = 0.5;
float p_reproduce_predateur = 0.5;
float p_manger = 0.7;
int temps_repousse_herbe = -15;


Animal *creer_animal(int x, int y, float energie) {
    Animal *pt_animal = (Animal*) malloc( sizeof(Animal) * 1 );
    assert( pt_animal != NULL );
    assert( 0 <= x && x < SIZE_X );
    assert( 0 <= y && y < SIZE_Y );

    pt_animal->x = x;
    pt_animal->y = y;
    pt_animal->energie = energie;
    pt_animal->dir[0] = rand() % 3 - 1;
    pt_animal->dir[1] = rand() % 3 - 1;
    pt_animal->suivant = NULL;
    return pt_animal;
}


Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
    if (animal == NULL) return liste;

    animal->suivant = liste;
    return animal;
}


unsigned int compte_animal_rec(Animal *la) {
    if (la == NULL) return 0;
    return 1 + compte_animal_rec(la->suivant);
}


unsigned int compte_animal_it(Animal *la) {
    int cpt = 0;

    while (la != NULL) {
        la = la->suivant;
        cpt++;
    }
    return cpt;
}


void afficher_ecosys(Animal *l_proies, Animal *l_predateurs) {
    char tab[SIZE_X][SIZE_Y];
    Animal *pt_animal = NULL;
    int nb_proies = 0;
    int nb_predateurs = 0;

    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            tab[i][j] = ' ';
        }
    }
    pt_animal = l_proies;
    while (pt_animal != NULL) {
        tab[ pt_animal->x ][ pt_animal->y ] = '*';
        nb_proies++;
        pt_animal = pt_animal->suivant;
    }
    pt_animal = l_predateurs;
    while (pt_animal != NULL) {
        if (tab[ pt_animal->x ][ pt_animal->y ] == '*') {
            tab[ pt_animal->x ][ pt_animal->y ] = '@';
        }
        else {
            tab[ pt_animal->x ][ pt_animal->y ] = 'O';
        }
        nb_predateurs++;
        pt_animal = pt_animal->suivant;
    }

    printf("Nb proies (*):    %6d\n", nb_proies);
    printf("Nb predateurs (O):%6d\n", nb_predateurs);
    printf("+");
    for (int j = 0; j < SIZE_Y; j++) {
        printf("-");
    }
    printf("+\n");
    for (int i = 0; i < SIZE_X; i++) {
        printf("|");
        for (int j = 0; j < SIZE_Y; j++) {
            printf("%c", tab[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (int j = 0; j < SIZE_Y; j++) {
        printf("-");
    }
    printf("+\n");
 
    return;
}


void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
    Animal *animal = NULL;

    assert(x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y);

    animal = creer_animal(x, y, energie);
    *liste_animal = ajouter_en_tete_animal(*liste_animal, animal);

    return;
}


void liberer_liste_animaux(Animal *liste) {
    if ( liste != NULL ) {
        liberer_liste_animaux( liste->suivant );
        free( liste );
    }

    return;
}


void enlever_animal(Animal **liste, Animal *animal) {
    Animal *pt_tmp = NULL;

    if (liste == NULL) return;
    if (*liste == NULL || animal == NULL) return;

    pt_tmp = *liste;
    if (pt_tmp == animal) {
        *liste = animal->suivant;
        free(animal);
        return;
    }
    while ( pt_tmp->suivant != NULL ) {
        if ( pt_tmp->suivant == animal) {
            pt_tmp->suivant = animal->suivant;
            free(animal);
            return;
        }
        pt_tmp = pt_tmp->suivant;
    }

    return;
}


void bouger_animaux(Animal *la) {
    while (la != NULL) {
        assert( la->x >= 0 && la->x < SIZE_X );
        assert( la->y >= 0 && la->y < SIZE_Y );
        if ( ((float)rand()) / RAND_MAX < p_ch_dir ) {
            la->dir[0] = rand() % 3 - 1;
            la->dir[1] = rand() % 3 - 1;
        }
        la->x = ( SIZE_X + la->x - la->dir[0] ) % SIZE_X;
        la->y = ( SIZE_Y + la->y - la->dir[1] ) % SIZE_Y;

        la = la->suivant;
    }

    return;
}


void reproduce(Animal **liste_animal, float p_reproduce) {
    assert( liste_animal != NULL );
    Animal *pta = *liste_animal;

    while (pta != NULL) {
        if ( ((float)rand()) / RAND_MAX < p_reproduce ) {
            ajouter_animal(pta->x, pta->y, pta->energie / 2., liste_animal);
            pta->energie /= 2.0;
        }
        pta = pta->suivant;
    }

    return;
}


void rafraichir_monde(int monde[SIZE_X][SIZE_Y]) {
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            monde[i][j] += 1;
        }
    }

    return;
}


void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    Animal *la = NULL;
    Animal *tmp = NULL;

    assert( liste_proie != NULL );
    la = *liste_proie;
    bouger_animaux(la);
    while (la != NULL) {
        la->energie -= 1.0;
        if ( monde[la->x][la->y] > 0 ) {
            la->energie += gain_energie_proie;
            monde[la->x][la->y] = temps_repousse_herbe;
        }
        tmp = la;
        la = la->suivant;
        if ( tmp->energie < 0.0 ) {
            enlever_animal( liste_proie, tmp);
        }
    }
    reproduce( liste_proie, p_reproduce_proie );

    return;
}


Animal *animal_en_XY(Animal *l, int x, int y) {
    while (l != NULL) {
        if ( l->x == x && l->y == y ) {
            return l;
        }
        l = l->suivant;
    }

    return NULL;
}


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
    Animal *la = NULL;
    Animal *tmp = NULL;
    Animal *proie = NULL;

    assert( liste_predateur != NULL );
    la = *liste_predateur;
    bouger_animaux(la);
    while (la != NULL) {
        la->energie -= 1.0;
        proie = animal_en_XY(*liste_proie, la->x, la->y);
        if ( proie != NULL ) {
            if ( ((float)rand()) / RAND_MAX < p_manger ) {
                la->energie += proie->energie; // gain_energie_predateur; 
                enlever_animal( liste_proie, proie );
            }
        }
        tmp = la;
        la = la->suivant;
        if ( tmp->energie < 0.0 ) {
            enlever_animal( liste_predateur, tmp);
        }
    }
    reproduce( liste_predateur, p_reproduce_predateur );

    return;
}




void clear_screen() {
    printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

