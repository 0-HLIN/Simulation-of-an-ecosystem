
#ifndef ECOSYS_H
#define ECOSYS_H

#define SIZE_X 20
#define SIZE_Y 60

/* Parametres grobaux de l'ecosysteme */
extern float p_ch_dir; //probabilite de changer de direction de deplacement
extern int gain_energie_proie;
extern int gain_energie_predateur;
extern float p_reproduce_proie;
extern float p_reproduce_predateur;
extern float p_manger;
extern int temps_repousse_herbe;


struct animal {
    int x;
    int y;
    double energie;
    int dir[2];
    struct animal *suivant;
};
typedef struct animal Animal;

Animal *creer_animal(int x, int y, float energie);

Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);

unsigned int compte_animal_rec(Animal *la);

unsigned int compte_animal_it(Animal *la);

void afficher_ecosys(Animal *l_proies, Animal *l_predateurs);

void ajouter_animal(int x, int y, float energie, Animal **list_animal);

void liberer_liste_animaux(Animal *liste);

void enlever_animal(Animal **liste, Animal *animal);

void bouger_animaux(Animal *la);

void reproduce(Animal **liste_animal, float p_reproduce);

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]);

void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]);

void clear_screen();

Animal *animal_en_XY(Animal *l, int x, int y);

void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie);

#endif
