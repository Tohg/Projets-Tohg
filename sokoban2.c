/**
* @file sokoban.c
* @brief On doit ranger les caisses sur les cibles
* @author Temeio HARAPOI--GAUDIN / 1B1
* @version version2
* @date 09/11/2025
*
* Nous sommes '@', nous pouvons pousser les caisses '$',
* De cette maniere nous devons les déplacer sur les '.'
* On ne peut pas pousser une caisse contre un mur
* On ne peut pas pousser plus de une caisse à la fois
*
*/

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

//definition des constantes
#define TAILLE 12
#define MOUVEMENT 1000
const char CHAR_SOKOBAN = '@';
#define CHAR_MUR '#'
const char CHAR_CAISSE = '$';
#define CHAR_VIDE ' '
#define CHAR_CIBLE '.'
#define CHAR_SOKOBAN_CIBLE '+'
#define CHAR_CAISSE_CIBLE '*'
#define OUI 'y'
#define NON 'n'
#define HAUT 'z'
#define GAUCHE 'q'
#define BAS 's'
#define DROITE 'd'
#define RECOMMENCER 'r'
#define ABANDONNER 'x'
#define ZOOM_PLUS '+'
#define ZOOM_MOINS '-'
#define ZOOM_MIN 1
#define ZOOM_MAX 3
#define ANNULER 'u'

typedef char t_Plateau[TAILLE][TAILLE];
typedef char t_tabDeplacement[MOUVEMENT];
void charger_partie(t_Plateau plateau, char fichier[15]);
void enregistrer_deplacements(t_tabDeplacement t, int nb, char fic[]);
void chargerDeplacements(typeDeplacements t, char fichier[], int * nb);
void afficher_plateau(t_Plateau plateau);
void afficher_entete(char fichier[], int compteur);
bool gagne(t_Plateau plateau);
void deplacer(t_Plateau plateau,int *lig, int *col,int *compteur, 
  char touche,t_tabDeplacement deplacement, t_Plateau plateauInitial);
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2);
void recherche_sokoban(t_Plateau plateau,int *lig,int *col);
void detecter_touche(char touche,int *lig,int *col,int *compteur,
  t_Plateau plateauInitial, t_Plateau plateau,bool *abandon,int *zoom, t_tabDeplacement deplacement);

int main() {
  //definition des variables
  char fichier[15];
  char nomdeplacement[15];
  int compteur = 0;
  char touche = '\0';
  int lig = 0;
  int col = 0;
  //definition du plateau
  t_Plateau plateauInitial = {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}};
  t_tabDeplacement deplacement;
  printf("Quelle fichier de partie : ");
  scanf("%14s", fichier);
  charger_partie(plateauInitial, fichier);
  //definition des deplacement
  printf("Quel fichier de déplacement : ")
  scanf("%14s", nomdeplacement);
  charger_deplacements(deplacement,nomdeplacement,&compteur, );
  //copie du niveau
  t_Plateau plateau;
  copie_plateau(plateauInitial,plateau);
  recherche_sokoban(plateau,&lig, &col);
  // dans la partie
  afficher_entete(fichier, compteur);
  afficher_plateau(plateau);
  return 0;
}

void charger_partie(t_Plateau plateau, char fichier[15]) {
  FILE *f;
  char finDeLigne;

  f = fopen(fichier, "r");
  if (f == NULL) {
    printf("FICHIER NON TROUVE");
    exit(EXIT_FAILURE);
  } else {
    for (int ligne = 0; ligne < TAILLE; ligne++) {
      for (int colonne = 0; colonne < TAILLE; colonne++) {
        fread(&plateau[ligne][colonne], sizeof(char), 1, f);
      }
      fread(&finDeLigne, sizeof(char), 1, f);
    }
    fclose(f);
  }
}

void chargerDeplacements(typeDeplacements t, char fichier[], int * nb){
    FILE * f;
    char dep;
    *nb = 0;

    f = fopen(fichier, "r");
    if (f==NULL){
        printf("FICHIER NON TROUVE\n");
    } else {
        fread(&dep, sizeof(char), 1, f);
        if (feof(f)){
            printf("FICHIER VIDE\n");
        } else {
            while (!feof(f)){
                t[*nb] = dep;
                (*nb)++;
                fread(&dep, sizeof(char), 1, f);
            }
        }
    }
    fclose(f);
}

/**
* @brief Permet d'afficher le plateau tout remplaçant l'affichage des caisses
*        sur cible et de sokoban sur cible par une caisse et sokoban
* @param plateau de type t_Plateau qui est un parametre d'entrée et va être 
*        affiché
* @param zoom qui va répéter les motifs dans un carré de taille zoom 
*/
void afficher_plateau(t_Plateau plateau,int zoom) {
  //affichage du plateau
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CHAR_CAISSE_CIBLE) {
        printf("%c",CHAR_CAISSE);
      } else if (plateau[i][j] == CHAR_SOKOBAN_CIBLE) {
        //on n'affiche pas la différence entre sokoban et sokoban-sur-une-
        // cible
        printf("%c",CHAR_SOKOBAN);
      } else {
        //on n'affiche pas la différence entre une caisse et une caisse-sur-
        // une-cible
        printf("%c", plateau[i][j]);
      }
    }}
    printf("\n");
  }


/**
* @brief Affiche l'entete au dessus du plateau avec les informations utiles
*        comme les touches, le nom du fichier ou le nombre de déplacement 
*        effectués
* @param fichier utilise le nom du fichier afin de savoir sur quelle grill 
*        on joue
* @param compteur Va afficher le nombre de coups effectuées depuis le début
*/
void afficher_entete(char fichier[], int compteur) { // affichage de l'entete
  system("clear");
  printf("=========== Sokoban ===========\n");
  printf("Fichier de la partie : ");
  printf("%s\n", fichier); // affichage du niveau
  printf("-------------------------------\n");
  printf("Nombre de coups effectué : %d\n", compteur); // affichage du compteur
  printf("-------------------------------\n");
}

/**
* @brief Permets de détecter si le joueur a gagné ou pas
* @param plateau Va prendre le plateau afin de vérifier si il 
*                reste des caisses qui ne sont pas sur des points cible
* @return True si le joueur a gagné, False dans le cas contraire
*/
bool gagne(t_Plateau plateau) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CHAR_CAISSE) {
        return false; //si il y a encore des caisses, ce n'est pas fini
      }
    }
  }
  return true; //c'est la victoire si il n'y a plus de caisse
}

/**
* @brief Permets de copier le plateau1 vers le plateau 2
* @param plateau1 plateau initial qui va être copié
* @param plateau2 nouveau plateau qui est la copie du plateau1
*/
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2){
  //copie de plateau1 vers plateau2
  for (int i= 0; i<TAILLE; i++){
    for (int j=0; j< TAILLE; j++){
        plateau2[i][j] = plateau1[i][j];
    }
  }
}

/**
* @brief Permets de trouver la position de sokoban et
*        d'en retourner la ligne et la colonne
* @param plateau plateau sur lequel on va rechercher 
*                CHAR_SOKOBAN ou CHAR_SOKOBAN_CIBLE
* @param lig parametre entrée/sortie qui va assigner
*            l'indice de la ligne où se trouve sokoban à lig
* @param col parametre entrée/sortie qui va assigner 
*            l'indice de la colonne où se trouve sokoban à col
*/
void recherche_sokoban(t_Plateau plateau,int  *lig,int *col){
  for(int i = 0; i < TAILLE; i++){
    for(int j = 0; j < TAILLE; j++){
      if ((plateau[i][j] == CHAR_SOKOBAN) || 
      (plateau[i][j] == CHAR_SOKOBAN_CIBLE)){
        *lig = i;
        *col = j;
      }
    }
  }
}

/**
* @brief Permets de déplacer sokoban en fonction de la touche pressée
*        et seulement si il a le droit de la faire
* @param plateau plateau qui va être mis à jour si sokoban peut bouger
*        en fonction
* @param lig parametre d'entrée/sortie qui va actualiser
*            la position de sokoban (sur la ligne)
* @param col parametre d'entrée/sortie qui va actualiser 
*            la position de sokoban (sur la colonne)
* @param compteur parametre d'entrée/sortie qui va 
*                 incrémenter le compteur si sokoban peut bouger
* @param touche parametre d'entrée qui va permettre de savoir dans 
*               quelle direction on cherche à bouger sokoban
* @param plateauInitial va permettre de résoudre un problème en détectant les 
*        cibles à l'origine du plateau
*/
void deplacer(t_Plateau plateau, int *lig, int *col,int *compteur,
  char touche,t_tabDeplacement deplacement, t_Plateau plateauInitial) {
    int di = 0;
    int dj = 0; // Déplacements : i = lignes ; j = colonnes
    int caisse = 0;
    switch (touche) {
        case HAUT:
          di = -1; 
          break; // Aller en haut
        case BAS:     
          di = 1;  
          break; // Aller en bas
        case GAUCHE:  
          dj = -1; 
          break; // Aller à gauche
        case DROITE:  
          dj = 1;  
          break; // Aller à droite
        default:      
          return; // Si la touche pressée n'existe pas
    }
    int ti = *lig + di; // Nouvelle ligne cible
    int tj = *col + dj; // Nouvelle colonne cible
    char target = plateau[ti][tj]; // Case cible
    if (target == CHAR_MUR) { // Vérification si le mouvement est possible
        return; // Mur : on ne bouge pas
    }
    // Déplacement simple (case vide ou cible)
    if (target == CHAR_VIDE || target == CHAR_CIBLE) {
        plateau[ti][tj] = (target == CHAR_CIBLE) ? CHAR_SOKOBAN_CIBLE :
         CHAR_SOKOBAN; // Mettre à jour la nouvelle position du joueur
    }
    // Tentative de pousser une caisse
    else if (target == CHAR_CAISSE || target == CHAR_CAISSE_CIBLE) {
        int bi = ti + di; // Nouvelle ligne de la caisse
        int bj = tj + dj; // Nouvelle colonne de la caisse
        char beyond = plateau[bi][bj]; // Case au-delà de la caisse
        // Vérifier si on peut pousser la caisse
        if (beyond == CHAR_VIDE || beyond == CHAR_CIBLE) {
          plateau[bi][bj] = (beyond == CHAR_CIBLE) ? CHAR_CAISSE_CIBLE
           : CHAR_CAISSE;           // Déplacer la caisse
          plateau[ti][tj] = (target == CHAR_CAISSE_CIBLE) ? CHAR_SOKOBAN_CIBLE
           : CHAR_SOKOBAN; // Mettre à jour la nouvelle position du joueur
          caisse = 1;//Mettre à jour la variable caisse pour l'enregistrement
        } else {
            return; // Impossible de pousser la caisse
        }
    } else {
        return; // Autres caractères : pas de déplacement
    }
    // Nettoyer l'ancienne position du joueur :
    // si la case était une cible à l'origine, remettre une cible,
    // sinon remettre un vide.
    plateau[*lig][*col] = ((plateauInitial[*lig][*col] == CHAR_CIBLE)||
    ((plateauInitial[*lig][*col] == CHAR_CAISSE_CIBLE)||
    ((plateauInitial[*lig][*col] == CHAR_SOKOBAN_CIBLE)))) 
    ? CHAR_CIBLE : CHAR_VIDE;
    *lig = ti; // Mettre à jour les coordonnées 
    *col = tj; //du joueur si le mouvement est valide
    enregistrer_tab_deplacement(touche, deplacement, caisse, *compteur);
    *compteur=*compteur+1;//augmente le compteur que si le mouvement est valide
}

/**
* @brief permets de rediriger vers les procédures adaptés en 
*        fonction de la touche
* @param touche va influencer la procédure lancé
* @param lig va actualiser la ligne sur laquelle est sokoban
* @param col va actualiser la colonne sur laquelle est sokoban
* @param compteur va actualiser le compteur de coup
* @param plateauInitial 
* @param plateau
* @param abandon
* @param zoom
* @param deplacement
*/
void detecter_touche(char touche,int *lig,int *col,int *compteur,
  t_Plateau plateauInitial,t_Plateau plateau,bool *abandon,int *zoom,
   t_tabDeplacement deplacement){
    switch(touche){
    case HAUT:
    case BAS:
    case GAUCHE:
    case DROITE:
      deplacer(plateau, lig, col, compteur, touche, deplacement, plateauInitial);
      break;
    case ABANDONNER: 
      *abandon = true;
      break;
    case ZOOM_MOINS:
    case ZOOM_PLUS:
      zoomer(touche, zoom);
      break;
    case RECOMMENCER:
      recommencer(plateauInitial, plateau, compteur, lig, col);
      break;
    case ANNULER:
      if (*compteur > 0) {
        deplacement_inverse(plateau, lig, col, compteur,
           deplacement[*compteur - 1], plateauInitial);
      }
      break;
  }
}
