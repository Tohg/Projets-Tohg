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
void enregistrer_partie(t_Plateau plateau, char fichier[15]);
void enregistrer_deplacements(t_tabDeplacement t, int nb, char fic[]);
//void charger_deplacements(t_tabDeplacement t, int *nb, char fichier[]);
int kbhit();
void afficher_plateau(t_Plateau plateau,int zoom);
void afficher_entete(char fichier[], int compteur);
bool gagne(t_Plateau plateau);
void deplacer(t_Plateau plateau,int *lig, int *col,int *compteur, 
  char touche,t_tabDeplacement deplacement, t_Plateau plateauInitial);
void copie_plateau(t_Plateau plateau1, t_Plateau plateau2);
void recherche_sokoban(t_Plateau plateau,int *lig,int *col);
void detecter_touche(char touche,int *lig,int *col,int *compteur,
  t_Plateau plateauInitial, t_Plateau plateau,bool *abandon,int *zoom, t_tabDeplacement deplacement);
void recommencer(t_Plateau plateauInitial, t_Plateau plateau,int *compteur,int *lig,int *col);
void zoomer(char touche,int *zoom);
void enregistrer_tab_deplacement(char touche, t_tabDeplacement deplacement,int caisse, int compteur);
void deplacement_inverse(t_Plateau plateau, int *lig, int *col,int *compteur,char touche, t_Plateau plateauInitial);
void bouger_annulation(t_Plateau plateau,int caisse, int ancien_lig, int ancien_col,int lig,int col,int pci, int pcj, t_Plateau plateauInitial);


int main() {
  //definition des variables
  char fichier[15];
  char nomdeplacement[15];
  char nom[15] = "";
  int compteur = 0;
  char touche = '\0';
  bool abandon = false;
  char enregistement = NON;
  char sauvegarde_deplacement = NON;
  int lig = 0;
  int col = 0;
  int zoom = 1;
  //definition du plateau
  t_Plateau plateauInitial = {{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}};
  t_tabDeplacement deplacement;
  printf("Quelle fichier de partie : ");
  scanf("%14s", fichier);
  charger_partie(plateauInitial, fichier);
  //copie du niveau
  t_Plateau plateau;
  copie_plateau(plateauInitial,plateau);
  recherche_sokoban(plateau,&lig, &col);
  // dans la partie
  while ((gagne(plateau) == false) && (abandon == false)) {
    afficher_entete(fichier, compteur);
    afficher_plateau(plateau, zoom);
    while (kbhit() == 0) {
    }
    touche = getchar();
    detecter_touche(touche,&lig,&col,&compteur,plateauInitial,plateau,&abandon,
      &zoom,deplacement);
  }
  afficher_entete( fichier, compteur);
  afficher_plateau(plateau, zoom);
  if (abandon == true) {
    //si on a abandonné, on demande si la personne veut sauvegarder sa 
    // progression
    printf("Voulez vous enregistrer la partie (y = oui / n = non) : ");
    scanf("%c", &enregistement);
    if (enregistement == OUI) { //si oui, on demande le nom attribué
      printf("Quel nom voulez vous lui donner : ");
      scanf("%14s", nom);
      enregistrer_partie(plateau, nom);
      printf("votre partie est bien enregistrée sous le nom %s \n", nom);
    } else {
      printf("Merci d'avoir joué\n");
    }
  }
  if (gagne(plateau) == true) {
    printf("Félicitation pour avoir su ranger ces caisses en %d coups\n",
      compteur);
  }
  printf("Voulez vous enregistrer vos déplacements ? (y = oui/n = non)\n");
  scanf(" %c", &sauvegarde_deplacement);
  if (sauvegarde_deplacement == OUI) {
    printf("Quel nom voulez vous lui donner : ");
    scanf("%14s", nomdeplacement);
    enregistrer_deplacements(deplacement, compteur, nomdeplacement);
    printf("Vos déplacements ont bien été enregistrés dans le fichier %s\n",
      nomdeplacement);
  }
  return 0;
}

void charger_partie(t_Plateau plateau, char fichier[15]) {
  FILE *f;
  char finDeLigne;

  f = fopen(fichier, "r");
  if (f == NULL) {
    printf("ERREUR SUR FICHIER");
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

void enregistrer_partie(t_Plateau plateau, char fichier[15]) {
  FILE *f;
  char finDeLigne = '\n';

  f = fopen(fichier, "w");
  for (int ligne = 0; ligne < TAILLE; ligne++) {
    for (int colonne = 0; colonne < TAILLE; colonne++) {
      fwrite(&plateau[ligne][colonne], sizeof(char), 1, f);
    }
    fwrite(&finDeLigne, sizeof(char), 1, f);
  }
  fclose(f);
}

void enregistrer_deplacements(t_tabDeplacement t, int nb, char fic[]){
    FILE * f;

    f = fopen(fic, "w");
    fwrite(t,sizeof(char), nb, f);
    fclose(f);
}

//void charger_deplacements(t_tabDeplacement t, int *compteur, char fichier[]){
//  FILE *f;
//
//  f=open(fichier, "r");
//  if (f=NULL){
//    printf("ERREUR SUR LE FICHIER DE DEPLACEMENT");
//    exit(EXIT_FAILURE);
//  } else {
//    *compteur = 0;
//    fread(&t[*compteur],sizeof(char),1,f);
//    while (! feof(f)){
//      *compteur = *compteur + 1;
//      fread(&t[*compteur],sizeof(char),1,f);
//    }
//    
//  }
//  fclose(f);
//}

int kbhit() {
  // la fonction retourne :
  // 1 si un caractere est present
  // 0 si pas de caractere présent
  int unCaractere = 0;
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

  if (ch != EOF) {
    ungetc(ch, stdin);
    unCaractere = 1;
  }
  return unCaractere;
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
    for(int x=0;x<zoom;x++){
    for (int j = 0; j < TAILLE; j++) {
      for(int y=0;y<zoom;y++){
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
  }}
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
  printf("=== Sokoban ===\n");
  printf("Fichier de la partie : ");
  printf("%s\n", fichier); // affichage du niveau
  printf("----------------\n");
  printf("Nombre de coups effectué : %d\n", compteur); // affichage du compteur
  printf("Liste des commandes :\n"); // affichage des différents coups
  printf("z : haut\n");
  printf("s : bas\n");
  printf("q : gauche\n");
  printf("d : droite\n");
  printf("+ : zoomer\n");
  printf("- : dézoomer\n");
  printf("u : annuler son coup\n");
  printf("x : abandonner la partie\n");
  printf("r : recommencer la partie\n");
  printf("----------------\n");
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

/**
* @brief recharge le plateau à partir de celui 
*.       de base
* @param plateauInitial plateau initialement
*        chargé 
* @param plateau plateau modifié par les
*        mouvement
* @param compteur va réinitialiser le 
*.       compteur
* @param lig va remettre la ligne a la position 
*.       initiale de sokoban
* @param col va remettre la colonne a la position 
*.       initiale de sokoban
*/
void recommencer(t_Plateau plateauInitial, t_Plateau plateau,int *compteur,
  int *lig,int *col){
    char confirmation = NON;
    printf("Êtes-vous sûr de vouloir recommencer ? (y = oui / n = non) : ");
    scanf(" %c", &confirmation);
    if (confirmation == OUI){
      //si l'on veut recommencer, on repart du plateauInitial
      copie_plateau(plateauInitial,plateau);
      *lig = 0;
      *col = 0;
      recherche_sokoban(plateau, lig, col);
      *compteur = 0;
    }
}

/**
* @brief 
* @param touche
* @param zoom
*/
void zoomer(char touche,int *zoom){
  if (touche == ZOOM_PLUS){
    if (*zoom < ZOOM_MAX){
      (*zoom)++;
    }
  }
  else if (touche == ZOOM_MOINS){
    if (*zoom > ZOOM_MIN) {
      (*zoom)--;
    }
  }
}

/**
* @brief permets d'enregistrer le coup effectué en fonction de la
*        touche pressé et de si le coup a poussé une caisse
* @param touche va influencer le caractere enregistré
* @param deplacement tableau dans lequel sont enregistrés les coups
* @param caisse va influencer si la case est une majuscule ou non
*        en fonction de si l'on pousse une caisse ou non
* @param compteur permets d'être bien rendu au bon endroit pour 
*        enregistrer le coup
*/
void enregistrer_tab_deplacement(char touche, t_tabDeplacement deplacement,
  int caisse, int compteur){
  switch(touche){
    case GAUCHE:
      touche = 'g';
      break;
    case HAUT:
      touche = 'h';
      break;
    case DROITE:
      touche = 'd';
      break;
    case BAS:
      touche = 'b';
      break; 
  }
  if (caisse == 1){
    touche = toupper(touche);
  }
  deplacement[compteur] = touche;
}

/**
* @brief Permets d'annuler son coup et ainsi donc
*        de décrémenter le compteur
* @param plateau va mettre à jour le plateau vers
*        la position précédente
* @param lig va remettre l'indice ligne à la position
*        précédente de sokoban
* @param col va remettre l'indice colonne à la position
*        précédente de sokoban
* @param compteur va décrémenter celui ci afin de ne pas
*        comptabiliser le coup précédement effectué
* @param touche c'est la derniere case du tableau t_tabDeplacement pour savoir
*        quelle action annuler
* @param plateauInitial va permettre de résoudre un problème en détectant les 
*        cibles à l'origine du plateau
*/
void deplacement_inverse(t_Plateau plateau, int *lig, int *col,
  int *compteur,char touche, t_Plateau plateauInitial){
  int dj = 0;
  int di = 0;
  int pcj = 0;
  int pci = 0;
  int caisse = 0;
  int ancien_lig = 0;
  int ancien_col = 0;
  
  switch(touche){
    case 'h':
    case 'H':
      di = -1;
      pci = 1;
      break;
    case 'd':
    case 'D':
      dj = 1;
      pcj = -1;
      break;
    case 'b':
    case 'B':
      di = 1;
      pci = 1;
      break;
    case 'g':
    case 'G':
      dj = -1;
      pcj = 1;
      break;
  }
  
  // Position où Sokoban doit revenir
  ancien_lig = *lig - di;
  ancien_col = *col - dj;
  
  // Position où la caisse a été poussée
  pci = *lig + di;
  pcj = *col + dj;
  
  if (pci < 0 || pci >= TAILLE || pcj < 0 || pcj >= TAILLE) {
    return;
  }
  
  if ((touche == 'H')||(touche == 'B')||(touche == 'G')||(touche == 'D')) { 
    caisse = 1;
  }
    bouger_annulation(plateau, caisse, ancien_lig, ancien_col, *lig, *col,
      pci, pcj, plateauInitial);
  
  *compteur = *compteur - 1;
  *col = ancien_col;
  *lig = ancien_lig;
}

/**
* @brief procédure utilisé dans deplacement_inverse() pour annuler le
*        coup précédent
* @param plateau va mettre à jour le plateau vers
*        la position précédente
* @param caisse permet de savoir si le coup précedement effectué a poussé
*               une caisse
* @param ancien_lig ligne ou sokoban doit revenir
* @param ancien_col colonne ou sokoban doit revenir
* @param lig ligne ou est à l'instant sokoban
* @param col colonne ou est à l'instant sokoban
* @param pci indice de la ligne ou la caisse a été poussée
* @param pcj indice de la colonne ou la caisse a été poussée
* @param plateauInitial va permettre de résoudre un problème en détectant les 
*        cibles à l'origine du plateau
*/
void bouger_annulation(t_Plateau plateau,int caisse, int ancien_lig,
   int ancien_col,int lig,int col,int pci, int pcj, t_Plateau plateauInitial){
  if (caisse == 1) {
    // Cas avec caisse
    // Remettre Sokoban à sa position avant (ancien_lig, ancien_col)
    plateau[ancien_lig][ancien_col] = 
    ((plateauInitial[ancien_lig][ancien_col] == CHAR_CIBLE)||
    (plateauInitial[ancien_lig][ancien_col] == CHAR_CAISSE_CIBLE)||
    (plateauInitial[ancien_lig][ancien_col] == CHAR_SOKOBAN_CIBLE)) 
    ? CHAR_SOKOBAN_CIBLE : CHAR_SOKOBAN;
    // Remettre la caisse à sa position avant la poussée (où Sokoban est 
    // actuellement)
    plateau[lig][col] = ((plateauInitial[lig][col] == CHAR_CIBLE)||
    (plateauInitial[lig][col] == CHAR_CAISSE_CIBLE)||
    (plateauInitial[lig][col] == CHAR_SOKOBAN_CIBLE)) 
    ? CHAR_CAISSE_CIBLE : CHAR_CAISSE;
    // Nettoyer la position où la caisse avait été poussée :
    // si la case était une cible à l'origine, remettre une cible sinon vide.
    plateau[pci][pcj] = ((plateauInitial[pci][pcj] == CHAR_CIBLE)
    ||(plateauInitial[pci][pcj] == CHAR_CAISSE_CIBLE)
    ||(plateauInitial[pci][pcj] == CHAR_SOKOBAN_CIBLE)) 
    ? CHAR_CIBLE : CHAR_VIDE;
  } else {
    // Cas sans caisse
    plateau[ancien_lig][ancien_col] = 
    ((plateauInitial[ancien_lig][ancien_col] == CHAR_CIBLE)||
    (plateauInitial[ancien_lig][ancien_col] == CHAR_CAISSE_CIBLE)||
    (plateauInitial[ancien_lig][ancien_col] == CHAR_SOKOBAN_CIBLE))
     ? CHAR_SOKOBAN_CIBLE : CHAR_SOKOBAN;
    // Nettoyer la position actuelle
    plateau[lig][col] = ((plateauInitial[lig][col] == CHAR_CIBLE)||
    (plateauInitial[lig][col] == CHAR_CAISSE_CIBLE)||
    (plateauInitial[lig][col] == CHAR_SOKOBAN_CIBLE)) 
    ? CHAR_CIBLE : CHAR_VIDE;
  }
}
