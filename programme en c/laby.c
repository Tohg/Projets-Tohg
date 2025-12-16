#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

////////////////////////////
// CONSTANTES
////////////////////////////

// taille du labyrinthe carré
#define TAILLE 20

// liste des symboles
#define MUR         '#'
#define PERSONNAGE  '@'
#define VIDE        ' '
#define MUR_AFFICHAGE "█"   // Attention, c'est bien une chaine de caractères

// liste des touches gérées par le jeu
#define GAUCHE   'q'
#define HAUT     'z'
#define BAS      's'
#define DROITE   'd'
#define ABANDON  'x'

// position initiale du personnage (entrée du labyrinthe)
#define L_ENTREE 1
#define C_ENTREE 0

// position de la sortie
#define L_SORTIE TAILLE-2
#define C_SORTIE TAILLE-1


////////////////////////////
// DECLARATION DES TYPES
////////////////////////////
typedef char t_labyrinthe[TAILLE][TAILLE];


////////////////////////////
// PROTOTYPES DES FONCTIONS
////////////////////////////
int kbhit();
void afficher_entete(int nbMouv, int nbcoll, char fichier[]);
void afficher_labyrinthe(t_labyrinthe laby);
void charger_labyrinthe(t_labyrinthe laby, char fichier[]);
bool deplacer_personnage(t_labyrinthe laby, int* l, int*y, char dir, int * nbMouv, int * nbColl);


////////////////////////////
// PROGRAMME PRINCIPAL
////////////////////////////
int main(){
    t_labyrinthe leLabyrinthe;

    int lPersonnage; // coordonnées
    int cPersonnage; // du personnage
    bool gagne;
    bool abandon;
    char touche;
    int nbMouvements;
    int nbCollisions;
    char nomFichier[50] = "labyrinthe.laby";

    charger_labyrinthe(leLabyrinthe, nomFichier);

    //initialisation
    lPersonnage = 1;
    cPersonnage = 0;
    gagne = false;
    abandon = false;
    touche = VIDE;
    nbMouvements = 0;
    nbCollisions = 0;

    // boucle de jeu
    while ( (abandon == false)&&(gagne == false)  ){
        afficher_entete(nbMouvements, nbCollisions, nomFichier);
        afficher_labyrinthe(leLabyrinthe);
        while (kbhit() == 0) {
        }
        touche = getchar();
        if (touche == ABANDON) {
            abandon = true;
        }
        gagne = deplacer_personnage(leLabyrinthe, &lPersonnage, &cPersonnage, touche, &nbMouvements, &nbCollisions);
    }

    // affichage du résultat
    afficher_entete(nbMouvements, nbCollisions, nomFichier);
    afficher_labyrinthe(leLabyrinthe);
    if (abandon == true) {
        printf("ABANDON\n");
    }
    if ((gagne == true) && (nbCollisions != 0)) {
        printf("SORTIE TROUVEE MAIS AVEC COLLISION\n");
    }
    else if (gagne==true) {
        printf("VICTOIRE\n");
    }
    return EXIT_SUCCESS;
}


/////////////////////////////////////
// AUTRES FONCTIONS ET PROCEDURES
/////////////////////////////////////
void afficher_entete(int nbMouv, int nbColl, char nomFichier[]){
    // cette procédure efface l'écran puis affiche :
    //  - le nombre de mouvements effectués
    //  - et le nombre de collisions subies
    system("clear");
    printf("-----------------------------------------\n");
    printf("L A B Y R I N T H E\n\n");
    printf("Collisions : %d\n\n", nbColl);
    printf("Nombre de coups effectué : %d\n", nbMouv);
    printf("-----------------------------------------\n");

}

void afficher_labyrinthe(t_labyrinthe laby){
    // cette procédure affiche à l'écran les éléments du labyrinthe. Attention :
    //  - tout symbole doit être affiché en double
    //  - et le symbole du mur est en fait une chaine de caractère
        
    for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      for(int y=0;y<2;y++){
      if (laby[i][j] == MUR) {
        printf("%s",MUR_AFFICHAGE);
      } else {
        printf("%c", laby[i][j]);
      }
    }}
    printf("\n");
  }}



bool deplacer_personnage(t_labyrinthe laby, int * l, int * c, char dir, int * nbMouv, int * nbColl){
    // cette fonction tente de déplacer le personnage dans le labyrinthe
    // *l et *c : ligne et colonne où se situe le personnage
    // dir : sens du déplacement souhaité (HAUT, BAS, GAUCHE ou DROITE)
    // *nbMouv : nombre de déplacements effectués
    // *nbColl : nombre de collisions 
    //
    // la fonction retourne true si le personnage a atteint la sortie, et false sinon

    int lSuivante;
    int cSuivante;
    bool gagne = false;

    // calcule la case voisine de celle du personnage en fonction de la direction
    lSuivante = *l;
    cSuivante = *c;
    
    switch (dir){
        case GAUCHE :
            if (*c>0) cSuivante = (*c)-1;
            break;
        case HAUT :
            if (*l>0)lSuivante = (*l)-1;
            break;
        case BAS :
            if (*l<TAILLE-1)lSuivante = (*l)+1;   
            break;
        case DROITE :
            if (*c<TAILLE-1) cSuivante = (*c)+1;
            break;
    }

    if (laby[lSuivante][cSuivante] == MUR) { // Vérification si le mouvement est possible
        *nbColl = *nbColl + 1;
    }
    else{
        *nbMouv = *nbMouv + 1;
        laby[lSuivante][cSuivante] = PERSONNAGE;
        laby[*l][*c] = VIDE;
        *l = lSuivante;
        *c = cSuivante;
        if ((*l == TAILLE-2)&&(*c == TAILLE-1)){
            gagne = true;
        }
    }
   
    return gagne;
}

void charger_labyrinthe(t_labyrinthe laby, char fichier[]){
    // cette procédure lit les caractères d'un fichier dont le nom est en paramètre
    // et les recopie dans le labyrinthe passé en paramètre
    FILE * f;
    char finDeLigne;

    f = fopen(fichier, "r");
    if (f==NULL){
        printf("ERREUR SUR FICHIER");
        exit(EXIT_FAILURE);
    } else {
        for (int ligne=0 ; ligne<TAILLE ; ligne++){
            for (int colonne=0 ; colonne<TAILLE ; colonne++){
                fread(&laby[ligne][colonne], sizeof(char), 1, f);
            }
            fread(&finDeLigne, sizeof(char), 1, f);
        }
        fclose(f);
    }
}

int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere présent
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

