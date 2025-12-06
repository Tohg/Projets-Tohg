/**
 * @file sokoban2.c
 * @brief On doit ranger les caisses sur les cibles
 * @author Temeio HARAPOI--GAUDIN / Noa PRIEM/ 1B1
 * @version version1
 * @date 02/12/2025
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
#include <unistd.h>

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
#define HAUT 'h'
#define GAUCHE 'g'
#define BAS 'b'
#define DROITE 'd'
#define HAUT_CAISSE 'H'
#define GAUCHE_CAISSE 'G'
#define BAS_CAISSE 'B'
#define DROITE_CAISSE 'D'

typedef char t_Plateau[TAILLE][TAILLE];
typedef char t_tabDeplacement[MOUVEMENT];
void charger_partie(t_Plateau plateau, char fichier[15]);
void charger_deplacements(t_tabDeplacement t, char fichier[], int *nb);
void afficher_plateau(t_Plateau plateau);
void afficher_entete(char fichier[], int compteur);
bool gagne(t_Plateau plateau);
void detecter_gagne(t_Plateau plateau);
void deplacer(t_Plateau plateau, int *lig, int *col, int *compteur,
              char touche, t_tabDeplacement deplacement,
              t_Plateau plateauInitial, int caisse);
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2);
void recherche_sokoban(t_Plateau plateau, int *lig, int *col);
bool obtenir_direction(char touche, int *di, int *dj);
char obtenir_caractere_affichage(char cible, int caisse);
char obtenir_caractere_ancienne_pos(char original);
bool verifier_mouvement_valide(char cible, int caisse);
void detecter_touche(char touche, int *lig, int *col, int *compteur,
                     t_Plateau plateauInitial, t_Plateau plateau,
                     t_tabDeplacement deplacement);

int main() {
  //definition des variables
  char fichier[15];
  char nomdeplacement[15];
  int compteur = 0;
  int tailleDep = 0;
  int lig = 0;
  int col = 0;
  char touche =' ';
  //definition du plateau
  t_Plateau plateauInitial = {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}};
  t_tabDeplacement deplacement;
  printf("Quelle fichier de partie : ");
  scanf("%14s", fichier);
  charger_partie(plateauInitial, fichier);
  //definition des déplacement
  printf("Quel fichier de déplacement : ");
  scanf("%14s", nomdeplacement);
  charger_deplacements(deplacement,nomdeplacement,&tailleDep);
  //copie du niveau
  t_Plateau plateau;
  copie_plateau(plateauInitial,plateau);
  recherche_sokoban(plateau,&lig, &col);
  // dans la partie
  for (int i = 0; i < tailleDep; i++) {
    usleep(500000);
    touche = deplacement[i];
    detecter_touche(touche, &lig, &col, &compteur,
                    plateauInitial, plateau, deplacement);
    afficher_entete(fichier, compteur);
    afficher_plateau(plateau);
  }
  detecter_gagne(plateau);
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

void charger_deplacements(t_tabDeplacement t, char fichier[],
                          int *nb) {
  FILE *f;
  char dep;
  *nb = 0;

  f = fopen(fichier, "r");
  if (f == NULL) {
    printf("FICHIER NON TROUVE\n");
  } else {
    fread(&dep, sizeof(char), 1, f);
    if (feof(f)) {
      printf("FICHIER VIDE\n");
    } else {
      while (!feof(f)) {
        t[*nb] = dep;
        (*nb)++;
        fread(&dep, sizeof(char), 1, f);
      }
    }
  }
  fclose(f);
}

/**
 * @brief Affiche le plateau en remplaçant les caisses/sokoban
 *        sur cible par leurs symboles normaux
 * @param plateau le plateau à afficher
 */
void afficher_plateau(t_Plateau plateau) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CHAR_CAISSE_CIBLE) {
        printf("%c", CHAR_CAISSE);
      } else if (plateau[i][j] == CHAR_SOKOBAN_CIBLE) {
        printf("%c", CHAR_SOKOBAN);
      } else {
        printf("%c", plateau[i][j]);
      }
    }
    printf("\n");
  }
}

/**
 * @brief Affiche l'entête du jeu avec infos utiles
 * @param fichier le nom du fichier de partie
 * @param compteur le nombre de coups effectués
 */
void afficher_entete(char fichier[], int compteur) {
  system("clear");
  printf("=========== Sokoban ===========\n");
  printf("Fichier de la partie : ");
  printf("%s\n", fichier);
  printf("-------------------------------\n");
  printf("Nombre de coups effectué : %d\n", compteur);
  printf("-------------------------------\n");
}

/**
 * @brief Détecte si le joueur a gagné ou pas
 * @param plateau le plateau à vérifier
 * @return True si gagné, False sinon
 */
bool gagne(t_Plateau plateau) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CHAR_CAISSE) {
        return false; // Si il y a encore des caisses, ce n'est pas fini
      }
    }
  }
  return true; // C'est la victoire si il n'y a plus de caisse
}

void detecter_gagne(t_Plateau plateau) {
  if (gagne(plateau)) {
    printf("Félicitations, les déplacements amènent "
           "à la solution !\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("Les déplacements n'amènent pas "
           "à la solution\n");
  }
}


/**
 * @brief Copie le plateau1 vers le plateau 2
 * @param plateau1 plateau source
 * @param plateau2 plateau destination
 */
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      plateau2[i][j] = plateau1[i][j];
    }
  }
}

/**
 * @brief Trouve la position de sokoban
 * @param plateau le plateau à fouiller
 * @param lig pointeur de ligne
 * @param col pointeur de colonne
 */
void recherche_sokoban(t_Plateau plateau, int *lig, int *col) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if ((plateau[i][j] == CHAR_SOKOBAN) || 
          (plateau[i][j] == CHAR_SOKOBAN_CIBLE)) {
        *lig = i;
        *col = j;
      }
    }
  }
}

/**
 * @brief Récupère les déplacements basés sur la touche
 * @param touche la touche pressée
 * @param di pointeur déplacement ligne
 * @param dj pointeur déplacement colonne
 * @return true si touche valide
 */
bool obtenir_direction(char touche, int *di, int *dj) {
  *di = 0;
  *dj = 0;
  switch (touche) {
    case HAUT:   *di = -1; return true;
    case BAS:    *di = 1;  return true;
    case GAUCHE: *dj = -1; return true;
    case DROITE: *dj = 1;  return true;
    default:     return false;
  }
}

/**
 * @brief Récupère le caractère à afficher
 * @param cible le caractère cible
 * @param caisse 1 si caisse, 0 sinon
 * @return le caractère à afficher
 */
char obtenir_caractere_affichage(char cible, int caisse) {
  if (caisse) {
    return (cible == CHAR_CIBLE) ? CHAR_CAISSE_CIBLE : CHAR_CAISSE;
  } else {
    return (cible == CHAR_CIBLE) ? CHAR_SOKOBAN_CIBLE : CHAR_SOKOBAN;
  }
}

/**
 * @brief Récupère le caractère pour l'ancienne position
 * @param original le caractère original
 * @return CHAR_CIBLE ou CHAR_VIDE
 */
char obtenir_caractere_ancienne_pos(char original) {
  return (original == CHAR_CIBLE || 
          original == CHAR_CAISSE_CIBLE || 
          original == CHAR_SOKOBAN_CIBLE) ? CHAR_CIBLE : CHAR_VIDE;
}

/**
 * @brief Vérifie si le mouvement est autorisé
 * @param cible la case cible
 * @param caisse 1 pour pousser caisse, 0 sinon
 * @return true si autorisé
 */
bool verifier_mouvement_valide(char cible, int caisse) {
  if (cible == CHAR_MUR) return false;
  if (caisse) {
    return (cible == CHAR_CAISSE || cible == CHAR_CAISSE_CIBLE);
  } else {
    return (cible != CHAR_CAISSE && cible != CHAR_CAISSE_CIBLE);
  }
}

/**
 * @brief Déplace sokoban en fonction de la touche
 * @param plateau plateau mis à jour
 * @param lig pointeur position ligne
 * @param col pointeur position colonne
 * @param compteur pointeur compteur de coups
 * @param touche direction du mouvement
 * @param deplacement inutilisé
 * @param plateauInitial plateau initial
 * @param caisse 1 pour pousser caisse
 */
void deplacer(t_Plateau plateau, int *lig, int *col, int *compteur,
              char touche, t_tabDeplacement deplacement,
              t_Plateau plateauInitial, int caisse) {
  int di = 0, dj = 0;
  
  if (!obtenir_direction(touche, &di, &dj))
    return;
  
  int ti = *lig + di;
  int tj = *col + dj;
  char cible = plateau[ti][tj];
  
  if (!verifier_mouvement_valide(cible, caisse))
    return;
  
  // Pousser une caisse si nécessaire
  if (caisse == 1) {
    int bi = ti + di;
    int bj = tj + dj;
    char au_dela = plateau[bi][bj];
    if (au_dela != CHAR_VIDE && au_dela != CHAR_CIBLE)
      return;
    plateau[bi][bj] = (au_dela == CHAR_CIBLE) ? CHAR_CAISSE_CIBLE :
     CHAR_CAISSE;
  }
  
  plateau[ti][tj] = obtenir_caractere_affichage(cible, caisse);
  plateau[*lig][*col] =
    obtenir_caractere_ancienne_pos(plateauInitial[*lig][*col]);
  
  *lig = ti;
  *col = tj;
  *compteur = *compteur + 1;
}

/**
 * @brief Redirige vers les procédures adaptées
 * @param touche la touche pressée
 * @param lig pointeur ligne sokoban
 * @param col pointeur colonne sokoban
 * @param compteur pointeur compteur coup
 * @param plateauInitial plateau initial
 * @param plateau plateau courant
 * @param deplacement déplacements
 */
void detecter_touche(char touche, int *lig, int *col, int *compteur,
                     t_Plateau plateauInitial, t_Plateau plateau,
                     t_tabDeplacement deplacement) {
  int caisse = 0;
  
  switch (touche) {
    case HAUT_CAISSE:
    case GAUCHE_CAISSE:
    case BAS_CAISSE:
    case DROITE_CAISSE:
      touche = tolower(touche);
      caisse = 1;
      break;
  }
  
  deplacer(plateau, lig, col, compteur, touche, deplacement, 
           plateauInitial, caisse);
}
