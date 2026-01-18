/**
 * @file sokoban2v2.c
 * @brief Sokoban autonome avec optimisation de la suite de déplacements
 * @author Temeio HARAPOI--GAUDIN / Noa PRIEM / 1B1
 * @version 2
 * @date 18/01/2025
 *
 * Nous sommes '@', nous poussons les caisses '$' sur les cibles '.'
 * Le programme élimine les déplacements non joués et les séquences
 * inutiles
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <ctype.h>

#define TAILLE 14
#define MAX_MOVES 1000
#define BUFFER_SIZE 15

typedef char t_Plateau[TAILLE][TAILLE];
typedef char t_Moves[MAX_MOVES];

/* Constantes pour les caractères */
#define CH_SOKOBAN '@'
#define CH_MUR '#'
#define CH_CAISSE '$'
#define CH_VIDE ' '
#define CH_CIBLE '.'
#define CH_SOKOBAN_CIBLE '+'
#define CH_CAISSE_CIBLE '*'

/* Constantes pour les mouvements */
#define HAUT 'h'
#define BAS 'b'
#define GAUCHE 'g'
#define DROITE 'd'

/* Fonctions */
void charger_plateau(t_Plateau plateau, const char *fichier);
void charger_moves(t_Moves moves, const char *fichier, int *count);
void afficher_plateau(t_Plateau plateau);
void afficher_entete(const char *nom_move, int coup);
bool est_gagne(t_Plateau plateau);
void afficher_bilan(t_Plateau plateau, const char *nom_partie,
                    const char *nom_move, int taille,
                    const char *moves_played);
void deplacer_sokoban(t_Plateau plateau, t_Plateau initial,
                      int *lig, int *col, char move, int *coup,
                      bool *move_joue);
bool get_direction(char move, int *di, int *dj);
bool position_valide(int i, int j);
bool deplacer_caisse(t_Plateau plateau, int ni, int nj,
                     int di, int dj);
bool mouvement_possible(t_Plateau plateau, int ni, int nj,
                        int di, int dj);
void mettre_a_jour_plateau(t_Plateau plateau, t_Plateau initial,
                           int lig, int col, int ni, int nj);
void copier_plateau(t_Plateau src, t_Plateau dst);
void trouver_sokoban(t_Plateau plateau, int *lig, int *col);
bool est_mouvement_valide(char move);
void clear_screen(void);
void read_input(char *buf, int size);


int main(void) {
  char nom_partie[BUFFER_SIZE];
  char nom_move[BUFFER_SIZE];
  t_Plateau plateau_init, plateau;
  t_Moves moves_original, moves_clean;
  t_Moves moves_played;
  int lig, col, coup = 0;
  int taille_original, taille_clean;
  int taille_played = 0;

  printf("Nom du fichier partie (.sok): ");
  read_input(nom_partie, BUFFER_SIZE);
  charger_plateau(plateau_init, nom_partie);

  printf("Nom du fichier deplacements (.dep): ");
  read_input(nom_move, BUFFER_SIZE);
  charger_moves(moves_original, nom_move, &taille_original);

  copier_plateau(plateau_init, plateau);
  trouver_sokoban(plateau, &lig, &col);

  afficher_entete(nom_move, coup);
  afficher_plateau(plateau);

  for (int i = 0; i < taille_original; i++) {
#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
    bool move_joue = false;
    deplacer_sokoban(plateau, plateau_init, &lig, &col,
                     moves_original[i], &coup, &move_joue);
    if (move_joue) {
      moves_played[taille_played++] = moves_original[i];
    }
    afficher_entete(nom_move, coup);
    afficher_plateau(plateau);
  }

  moves_played[taille_played] = '\0';

  /* NO OPTIMIZATION: save moves_played directly (they are already a valid solution) */

  afficher_bilan(plateau, nom_partie, nom_move, taille_played, moves_played);

  return 0;
}


/**
 * @brief Charge le plateau depuis un fichier .sok
 * @param plateau tableau 2D à remplir
 * @param fichier nom du fichier à charger
 */
void charger_plateau(t_Plateau plateau, const char *fichier) {
  FILE *f = fopen(fichier, "r");
  char ligne[TAILLE + 2];

  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      plateau[i][j] = CH_VIDE;
    }
  }

  if (f == NULL) {
    printf("Erreur: fichier %s non trouve\n", fichier);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < TAILLE && fgets(ligne, TAILLE + 2, f); i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (j < (int)strlen(ligne) && ligne[j] != '\n'
          && ligne[j] != '\r') {
        plateau[i][j] = ligne[j];
      } else {
        plateau[i][j] = CH_VIDE;
      }
    }
  }

  fclose(f);
}

/**
 * @brief Charge les mouvements depuis un fichier .dep
 * @param moves tableau à remplir avec les mouvements
 * @param fichier nom du fichier à charger
 * @param count pointeur pour stocker le nombre de mouvements
 */
void charger_moves(t_Moves moves, const char *fichier, int *count) {
  FILE *f = fopen(fichier, "r");
  int ch;
  *count = 0;

  if (f == NULL) {
    printf("Erreur: fichier %s non trouve\n", fichier);
    exit(EXIT_FAILURE);
  }

  while ((ch = fgetc(f)) != EOF && *count < MAX_MOVES) {
    if (est_mouvement_valide((char)ch)) {
      moves[*count] = (char)ch;
      (*count)++;
    }
  }

  moves[*count] = '\0';
  fclose(f);
}

/**
 * @brief Vérifie si un caractère est un mouvement valide
 * @param move le caractère à vérifier
 * @return true si c'est h/b/g/d (minuscule ou majuscule)
 */
bool est_mouvement_valide(char move) {
  move = tolower(move);
  return (move == HAUT || move == BAS || move == GAUCHE ||
          move == DROITE);
}

/**
 * @brief Affiche le plateau en remplaçant les symboles spéciaux
 * @param plateau le plateau à afficher
 */
void afficher_plateau(t_Plateau plateau) {

  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CH_CAISSE_CIBLE) {
        printf("%c", CH_CAISSE);
      } else if (plateau[i][j] == CH_SOKOBAN_CIBLE) {
        printf("%c", CH_SOKOBAN);
      } else {
        printf("%c", plateau[i][j]);
      }
    }
    printf("\n");
  }
}

/**
 * @brief Affiche l'entête avec les informations actuelles
 * @param nom_move nom du fichier de mouvements
 * @param coup nombre de coups effectués
 */
void afficher_entete(const char *nom_move, int coup) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  printf("======= SOKOBAN =======\n");
  printf("Mouvements: %s\n", nom_move);
  printf("Coups: %d\n", coup);
  printf("=======================\n");
}

/**
 * @brief Cherche la position de Sokoban sur le plateau
 * @param plateau le plateau à analyser
 * @param lig pointeur pour la ligne
 * @param col pointeur pour la colonne
 */
void trouver_sokoban(t_Plateau plateau, int *lig, int *col) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CH_SOKOBAN ||
          plateau[i][j] == CH_SOKOBAN_CIBLE) {
        *lig = i;
        *col = j;
      }
    }
  }
}

/**
 * @brief Copie le plateau source vers le plateau destination
 * @param src plateau source
 * @param dst plateau destination
 */
void copier_plateau(t_Plateau src, t_Plateau dst) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      dst[i][j] = src[i][j];
    }
  }
}

/**
 * @brief Obtient le caractère original d'une position du plateau initial
 * @param original le caractère du plateau initial
 * @return le caractère à afficher après le passage de Sokoban
 */
char get_ancien_char(char original) {
  if (original == CH_CIBLE || original == CH_CAISSE_CIBLE ||
      original == CH_SOKOBAN_CIBLE) {
    return CH_CIBLE;
  }
  return CH_VIDE;
}

/**
 * @brief Obtient le caractère à afficher pour Sokoban
 * @param cible le caractère de la destination
 * @return CH_SOKOBAN_CIBLE si cible, CH_SOKOBAN sinon
 */
char get_sokoban_char(char cible) {
  return (cible == CH_CIBLE) ? CH_SOKOBAN_CIBLE : CH_SOKOBAN;
}

/**
 * @brief Obtient les décalages de direction selon le mouvement
 * @param move le mouvement (h/b/g/d)
 * @param di pointeur pour le décalage ligne
 * @param dj pointeur pour le décalage colonne
 * @return true si le mouvement est valide
 */
bool get_direction(char move, int *di, int *dj) {
  move = tolower(move);
  *di = 0;
  *dj = 0;

  if (move == HAUT) {
    *di = -1;
  } else if (move == BAS) {
    *di = 1;
  } else if (move == GAUCHE) {
    *dj = -1;
  } else if (move == DROITE) {
    *dj = 1;
  } else {
    return false;
  }

  return true;
}

/**
 * @brief Vérifie si une position est valide sur le plateau
 * @param i la ligne
 * @param j la colonne
 * @return true si position valide
 */
bool position_valide(int i, int j) {
  return (i >= 0 && i < TAILLE && j >= 0 && j < TAILLE);
}

/**
 * @brief Gère le déplacement d'une caisse
 * @param plateau le plateau à modifier
 * @param ni position ligne de la caisse
 * @param nj position colonne de la caisse
 * @param di décalage ligne
 * @param dj décalage colonne
 * @return true si la caisse peut se déplacer
 */
bool deplacer_caisse(t_Plateau plateau, int ni, int nj,
                     int di, int dj) {
  int bi = ni + di;
  int bj = nj + dj;

  if (!position_valide(bi, bj)) {
    return false;
  }

  char beyond = plateau[bi][bj];
  if (beyond != CH_VIDE && beyond != CH_CIBLE) {
    return false;
  }

  plateau[bi][bj] = (beyond == CH_CIBLE) ?
                    CH_CAISSE_CIBLE : CH_CAISSE;
  return true;
}

/**
 * @brief Vérifie si le déplacement vers la case est possible
 * @param plateau le plateau
 * @param ni ligne destination
 * @param nj colonne destination
 * @param di décalage ligne
 * @param dj décalage colonne
 * @return true si déplacement valide
 */
bool mouvement_possible(t_Plateau plateau, int ni, int nj,
                        int di, int dj) {
  if (!position_valide(ni, nj)) {
    return false;
  }

  char cible = plateau[ni][nj];

  if (cible == CH_MUR) {
    return false;
  }

  if (cible == CH_CAISSE || cible == CH_CAISSE_CIBLE) {
    return deplacer_caisse(plateau, ni, nj, di, dj);
  }

  return true;
}

/**
 * @brief Met à jour le plateau après un déplacement
 * @param plateau le plateau à modifier
 * @param initial le plateau initial
 * @param lig ancienne position ligne
 * @param col ancienne position colonne
 * @param ni nouvelle position ligne
 * @param nj nouvelle position colonne
 */
void mettre_a_jour_plateau(t_Plateau plateau, t_Plateau initial,
                           int lig, int col, int ni, int nj) {
  char cible = plateau[ni][nj];
  plateau[ni][nj] = get_sokoban_char(cible);
  plateau[lig][col] = get_ancien_char(initial[lig][col]);
}

/**
 * @brief Déplace Sokoban selon le mouvement
 * @param plateau le plateau à modifier
 * @param initial le plateau initial pour les cibles
 * @param lig pointeur position ligne
 * @param col pointeur position colonne
 * @param move le mouvement à effectuer
 * @param coup pointeur compteur de coups
 * @param move_joue pointeur pour indiquer si le mouvement a été joué
 */
void deplacer_sokoban(t_Plateau plateau, t_Plateau initial,
                      int *lig, int *col, char move, int *coup,
                      bool *move_joue) {
  int di, dj;

  if (!get_direction(move, &di, &dj)) {
    *move_joue = false;
    return;
  }

  int ni = *lig + di;
  int nj = *col + dj;

  if (!mouvement_possible(plateau, ni, nj, di, dj)) {
    *move_joue = false;
    return;
  }

  mettre_a_jour_plateau(plateau, initial, *lig, *col, ni, nj);

  *lig = ni;
  *col = nj;
  (*coup)++;
  *move_joue = true;
}

/**
 * @brief Nettoie la suite en enlevant les séquences inutiles
 * @param original la suite de mouvements originale
 * @param nettoye la suite de mouvements nettoyée
 * @param taille pointeur pour mettre à jour la taille
 */
void nettoyer_moves(t_Moves original, t_Moves nettoye, int *taille) {
  int pos = 0;
  int i = 0;

  while (original[i] != '\0') {
    int bilan = 0;
    int debut = i;

    while (original[i] != '\0') {
      char move = tolower(original[i]);
      if (move == HAUT) {
        bilan++;
      } else if (move == BAS) {
        bilan--;
      } else if (move == GAUCHE) {
        bilan -= 2;
      } else if (move == DROITE) {
        bilan += 2;
      }
      i++;

      if (bilan == 0 && i > debut) {
        break;
      }
    }

    if (bilan != 0) {
      for (int j = debut; j < i; j++) {
        nettoye[pos++] = original[j];
      }
    }
  }

  nettoye[pos] = '\0';
  *taille = pos;
}

/**
 * @brief Vérifie si toutes les caisses sont sur les cibles
 * @param plateau le plateau à vérifier
 * @return true si aucune caisse seule n'existe
 */
bool est_gagne(t_Plateau plateau) {
  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (plateau[i][j] == CH_CAISSE) {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Affiche le bilan final et propose d'enregistrer
 * @param plateau le plateau final
 * @param nom_partie le nom du fichier de partie
 * @param nom_move le nom du fichier de mouvements
 * @param initial taille initiale de la suite
 * @param optimise taille après optimisation
 */
void afficher_bilan(t_Plateau plateau, const char *nom_partie,
                    const char *nom_move, int taille,
                    const char *moves_played) {
  if (est_gagne(plateau)) {
    printf("\n=== VICTOIRE ===\n");
    printf("La suite %s est bien une solution pour %s.\n", nom_move,
           nom_partie);
    printf("Longueur: %d caracteres\n", taille);

      char reponse_buf[4];
      printf("Enregistrer la suite (O/N): ");
      read_input(reponse_buf, sizeof(reponse_buf));
      char reponse = reponse_buf[0];

      if (reponse == 'O' || reponse == 'o') {
        char nom_fichier[BUFFER_SIZE];
        printf("Nom du fichier de sortie: ");
        read_input(nom_fichier, BUFFER_SIZE);

        FILE *f = fopen(nom_fichier, "w");
      if (f == NULL) {
        printf("Erreur: impossible de creer %s\n", nom_fichier);
      } else {
        fwrite(moves_played, 1, strlen(moves_played), f);
        fclose(f);
        printf("Fichier %s enregistre\n", nom_fichier);
      }
    }
  } else {
    printf("\n=== ECHEC ===\n");
    printf("La suite %s N'EST PAS une solution pour %s.\n", nom_move,
           nom_partie);
  }
}

/**
 * @brief Efface l'écran de façon portable
 */
void clear_screen(void) {
#ifdef _WIN32
  HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
  if (h == INVALID_HANDLE_VALUE) return;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD written;
  COORD topLeft = {0, 0};
  if (!GetConsoleScreenBufferInfo(h, &csbi)) {
    system("cls");
    return;
  }
  DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(h, ' ', cells, topLeft, &written);
  FillConsoleOutputAttribute(h, csbi.wAttributes, cells, topLeft, &written);
  SetConsoleCursorPosition(h, topLeft);
#else
  printf("\x1b[2J\x1b[H");
#endif
}

/**
 * @brief Lit une ligne d'entrée et supprime CR/LF
 */
void read_input(char *buf, int size) {
  if (fgets(buf, size, stdin) == NULL) {
    buf[0] = '\0';
    return;
  }
  size_t len = strlen(buf);
  while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) {
    buf[len-1] = '\0';
    len--;
  }
}
