/**
 * @file sokoban2.c
 * @brief On doit ranger les caisses sur les cibles
 * @author Temeio HARAPOI--GAUDIN / Noa PRIEM/ 1B1
 * @version version2
 * @date 02/12/2025
 *
 * Nous sommes '@', nous pouvons pousser les caisses '$',
 * De cette maniere nous devons les déplacer sur les '.'
 * On ne peut pas pousser une caisse contre un mur
 * On ne peut pas pousser plus de une caisse à la fois
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

//definition des constantes
#define TAILLE 12
#define MOUVEMENT 1000
#define FICHIER_BUFFER 15
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
#define TEMPS 250000
#define UP +1
#define DOWN -1
#define LEFT -2
#define RIGHT +2

 //définition des types
typedef char t_Plateau[TAILLE][TAILLE];
typedef char t_tabDeplacement[MOUVEMENT];

//définition des procédures et fonctions
void charger_partie(t_Plateau plateau, char fichier[15]);
void charger_deplacements(t_tabDeplacement t, char fichier[],
                          int *nb);
void afficher_plateau(t_Plateau plateau);
void afficher_entete(char fichier[], int compteur);
bool gagne(t_Plateau plateau);
void detecter_gagne(t_Plateau plateau, int compteur, char fichier[],
                    char deplacement[]);
void deplacer(t_Plateau plateau, int *lig, int *col, int *compteur,
              char touche, t_Plateau plateauInitial, int caisse);
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2);
void recherche_sokoban(t_Plateau plateau, int *lig, int *col);
bool verifier_mouvement_valide(char cible, int caisse);
bool obtenir_direction(char touche, int *di, int *dj);
char obtenir_caractere_affichage(char cible);
char obtenir_caractere_ancienne_pos(char original);
bool verifier_mouvement_valide(char cible, int caisse);
void detecter_touche(char touche, int *lig, int *col, int *compteur,
                     t_Plateau plateauInitial, t_Plateau plateau);
void nettoyer_deplacements(char deplacements[], char nettoyes[]);
int obtenir_bilan_mouvement(char touche);
bool mouvement_pousse_caisse(char touche);
int calculer_bilan_sequence(char deplacements[], int debut,
                            int *fin_sequence);


int main() {
  //definition des variables
  char fichier[FICHIER_BUFFER];
  char nomdeplacement[FICHIER_BUFFER];
  char copieDeplacement[MOUVEMENT];
  char depureeeDeplacement[MOUVEMENT];
  int compteur = 0;
  int tailleDep = 0;
  int lig = 0;
  int col = 0;
  char touche =' ';

  //definition du plateau
  t_Plateau plateauInitial = {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}};
  t_tabDeplacement deplacement;
  printf("Quel fichier de partie : ");
  scanf("%s", fichier);
  charger_partie(plateauInitial, fichier);

  //definition des déplacement
  printf("Quel fichier de déplacement : ");
  scanf("%s", nomdeplacement);
  charger_deplacements(deplacement, nomdeplacement, &tailleDep);
  strncpy(copieDeplacement, deplacement, MOUVEMENT);

  //Supprimer les séquences inutiles
  nettoyer_deplacements(copieDeplacement, depureeeDeplacement);
  strncpy(deplacement, depureeeDeplacement, MOUVEMENT);
  tailleDep = strlen(deplacement);

  //copie du niveau
  t_Plateau plateau;
  copie_plateau(plateauInitial, plateau);
  recherche_sokoban(plateau,&lig, &col);

  afficher_entete(nomdeplacement, compteur);
  afficher_plateau(plateau);

  for (int i = 0; i < tailleDep; i++) {
    usleep(TEMPS);
    touche = deplacement[i];
    detecter_touche(touche, &lig, &col, &compteur, plateauInitial,
                    plateau);
    afficher_entete(nomdeplacement, compteur);
    afficher_plateau(plateau);
  }

  detecter_gagne(plateau, compteur, fichier, nomdeplacement);
  return 0;
}

/**
 * @brief Charge le plateau de jeu depuis un fichier
 * @param plateau tableau 2D à remplir (12x12 caractères)
 * @param fichier nom du fichier contenant le plateau
 */
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

/**
 * @brief Charge la séquence de déplacements depuis un fichier
 * @param t tableau à remplir avec les touches de déplacement
 * @param fichier nom du fichier contenant les mouvements
 * @param nb pointeur pour stocker le nombre de mouvements lus
 */
void charger_deplacements(t_tabDeplacement t, char fichier[], int *nb) {
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
 * @param plateau le plateau à afficher (12x12)
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
 * @brief Vérifie si la partie est gagnée
 * @param plateau le plateau à vérifier
 * @return true si aucune caisse seule n'existe
 * @return false s'il reste au moins une caisse non placée
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

/**
 * @brief Affiche le résultat final et quitte le programme
 * @param plateau le plateau final à vérifier
 */
void detecter_gagne(t_Plateau plateau, int compteur, char fichier[],
                    char deplacement[]) {
  if (gagne(plateau)) {
    printf("La suite de déplacements %s est bien une solution ",
           fichier);
    printf("pour la partie %s. \n\nElle contient %d déplacements\n",
           deplacement, compteur);
  } else {
    printf("La suite de déplacement %s N'EST ", deplacement);
    printf("PAS une solution pour la partie %s .\n", fichier);
  }
}


/**
 * @brief Copie le plateau1 vers le plateau 2
 * @param plateau1 plateau source à copier
 * @param plateau2 plateau destination (12x12)
 */
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      plateau2[i][j] = plateau1[i][j];
    }
  }
}

/**
 * @brief Trouve la position actuelle de sokoban
 * @param plateau le plateau à fouiller (12x12)
 * @param lig pointeur où stocker la ligne trouvée
 * @param col pointeur où stocker la colonne trouvée
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
 * @brief Récupère les déplacements (di, dj) basés sur la touche
 * @param touche le caractère représentant la direction
 * @param di pointeur déplacement en ligne (-1=haut, +1=bas, 0=autre)
 * @param dj pointeur déplacement en colonne (-1=gauche, +1=droite)
 * @return true si la touche correspond à une direction valide
 * @return false pour touche invalide ou non reconnue
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
 * @brief Récupère le caractère à afficher pour sokoban
 * @param cible le caractère trouvé à la nouvelle position
 * @return CHAR_SOKOBAN_CIBLE (+) si cible était une cible
 * @return CHAR_SOKOBAN (@) sinon
 */
char obtenir_caractere_affichage(char cible) {
  return (cible == CHAR_CIBLE) ? CHAR_SOKOBAN_CIBLE : CHAR_SOKOBAN;
}

/**
 * @brief Récupère le caractère pour l'ancienne position de sokoban
 * @param original le caractère original du plateau initial
 * @return CHAR_CIBLE (.) si c'était une zone de cible
 * @return CHAR_VIDE ( ) sinon
 */
char obtenir_caractere_ancienne_pos(char original) {
  return (original == CHAR_CIBLE || original == CHAR_CAISSE_CIBLE ||
          original == CHAR_SOKOBAN_CIBLE) ? CHAR_CIBLE : CHAR_VIDE;
}

/**
 * @brief Vérifie si le mouvement vers la case est autorisé
 * @param cible le caractère trouvé à la case cible
 * @param caisse 1 si on essaie de pousser une caisse, 0 pour se déplacer
 * @return true si le mouvement est autorisé
 * @return false si impossible (mur, mauvais type de caisse, etc.)
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
 * @brief Déplace sokoban en fonction de la touche pressée
 * @param plateau le plateau modifié en place
 * @param lig pointeur position ligne actuelle (mise à jour)
 * @param col pointeur position colonne actuelle (mise à jour)
 * @param compteur pointeur compteur de coups (incrémenté)
 * @param touche caractère de direction (h/b/g/d)
 * @param plateauInitial plateau initial pour vérifier les cibles
 * @param caisse 1 pour pousser une caisse, 0 pour déplacer seul
 */
void deplacer(t_Plateau plateau, int *lig, int *col, int *compteur,
              char touche, t_Plateau plateauInitial, int caisse) {
  int di = 0, dj = 0;
  
  if (!obtenir_direction(touche, &di, &dj))
    return;
  
  int ti = *lig + di;
  int tj = *col + dj;
  char cible = plateau[ti][tj];

  if (!verifier_mouvement_valide(cible, caisse))
    return;

  int ancienne_lig = *lig;
  int ancienne_col = *col;

  if (caisse == 1) {
    int bi = ti + di;
    int bj = tj + dj;
    char au_dela = plateau[bi][bj];
    if (au_dela != CHAR_VIDE && au_dela != CHAR_CIBLE)
      return;
    plateau[bi][bj] = (au_dela == CHAR_CIBLE) ? CHAR_CAISSE_CIBLE
                                               : CHAR_CAISSE;
  }

  plateau[ti][tj] = obtenir_caractere_affichage(cible);
  plateau[ancienne_lig][ancienne_col] = obtenir_caractere_ancienne_pos(
      plateauInitial[ancienne_lig][ancienne_col]);
  
  *lig = ti;
  *col = tj;
  *compteur = *compteur + 1;
}

/**
 * @brief Analyse la touche et appelle deplacer avec bon paramètre
 * @param touche le caractère de touche lu (h/H/b/B/g/G/d/D)
 * @param lig pointeur position ligne sokoban (mise à jour)
 * @param col pointeur position colonne sokoban (mise à jour)
 * @param compteur pointeur compteur coup (mise à jour)
 * @param plateauInitial plateau initial (référence)
 * @param plateau plateau courant (modifié)
 */
void detecter_touche(char touche, int *lig, int *col, int *compteur,
                     t_Plateau plateauInitial, t_Plateau plateau) {
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
  
  deplacer(plateau, lig, col, compteur, touche, plateauInitial, caisse);
}

/**
 * @brief Retourne le bilan de déplacement d'un mouvement
 * @param touche le caractère représentant le mouvement
 * @return UP/DOWN/LEFT/RIGHT selon la direction, 0 si invalide
 */
int obtenir_bilan_mouvement(char touche) {
  switch (touche) {
    case HAUT:
    case HAUT_CAISSE:      return UP;
    case BAS:
    case BAS_CAISSE:       return DOWN;
    case GAUCHE:
    case GAUCHE_CAISSE:    return LEFT;
    case DROITE:
    case DROITE_CAISSE:    return RIGHT;
    default:               return 0;
  }
}

/**
 * @brief Vérifie si un mouvement pousse une caisse
 * @param touche le caractère représentant le mouvement
 * @return true si majuscule (avec caisse), false sinon
 */
bool mouvement_pousse_caisse(char touche) {
  return (touche == HAUT_CAISSE || touche == BAS_CAISSE ||
          touche == GAUCHE_CAISSE || touche == DROITE_CAISSE);
}

/**
 * @brief Calcule le bilan depuis une position jusqu'à annulation/caisse
 * @param deplacements la séquence de mouvements
 * @param debut position de départ
 * @param fin_sequence pointeur pour retourner la position de fin
 * @return le bilan total, ou 1 si caisse rencontrée avant annulation
 */
int calculer_bilan_sequence(char deplacements[], int debut,
                            int *fin_sequence) {
  int somme = obtenir_bilan_mouvement(deplacements[debut]);
  int i = debut + 1;

  while (deplacements[i] != '\0' && somme != 0) {
    if (mouvement_pousse_caisse(deplacements[i])) {
      *fin_sequence = i;
      return 1;
    }
    somme += obtenir_bilan_mouvement(deplacements[i]);
    i++;
  }

  *fin_sequence = i - 1;
  return somme;
}

/**
 * @brief Nettoie la séquence en supprimant les retours inutiles
 * @param deplacements séquence originale de mouvements
 * @param nettoyes séquence nettoyée sans retours inutiles
 */
void nettoyer_deplacements(char deplacements[], char nettoyes[]) {
  int position = 0;
  int i = 0;

  while (deplacements[i] != '\0') {
    if (mouvement_pousse_caisse(deplacements[i])) {
      nettoyes[position++] = deplacements[i];
      i++;
      continue;
    }

    int fin_seq;
    int bilan = calculer_bilan_sequence(deplacements, i, &fin_seq);

    if (bilan == 0 && fin_seq > i) {
      i = fin_seq + 1;
    } else {
      nettoyes[position++] = deplacements[i];
      i++;
    }
  }

  nettoyes[position] = '\0';
}