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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define TAILLE 12
#define MOUVEMENT 1000
#define FICHIER_BUFFER 15
#define TEMPS 250000

// Symboles et commandes
const char C_SOK = '@', C_MUR = '#', C_CAI = '$', C_VID = ' ', C_CIB = '.';
const char C_SOK_C = '+', C_CAI_C = '*';
const char H = 'h', G = 'g', B = 'b', D = 'd';
const char H_C = 'H', G_C = 'G', B_C = 'B', D_C = 'D';

typedef char t_Plateau[TAILLE][TAILLE];
typedef char t_tabDeplacement[MOUVEMENT];

// --- Prototypes ---
void charger_partie(t_Plateau p, char f[15]);
void charger_deplacements(t_tabDeplacement t, char f[], int *nb);
void afficher_plateau(t_Plateau p);
void afficher_entete(char f[], int c);
bool gagne(t_Plateau p);
void copie_plateau(t_Plateau p1, t_Plateau p2);
void recherche_sokoban(t_Plateau p, int *l, int *c);
bool obtenir_direction(char t, int *di, int *dj);
bool verifier_mouvement(char cible, int caisse);
bool deplacer(t_Plateau p, int *l, int *c, int *comp, char t, 
              t_Plateau pI, int cai);
bool traiter_entree(char t, int *l, int *c, int *comp, t_Plateau pI, 
                    t_Plateau p);
void optimiser(t_tabDeplacement in, int tIn, t_tabDeplacement out, int *tOut);
void enregistrer(t_tabDeplacement s, int t);

int main() {
    char f_partie[FICHIER_BUFFER], f_dep[FICHIER_BUFFER];
    int comp = 0, tDep = 0, l = 0, col = 0, nbJ = 0;
    t_tabDeplacement dep, sJouee, sOpti;
    t_Plateau pI, p;

    printf("Fichier partie : "); scanf("%s", f_partie); // [cite: 11]
    charger_partie(pI, f_partie);
    printf("Fichier déplacements : "); scanf("%s", f_dep); // [cite: 12]
    charger_deplacements(dep, f_dep, &tDep);

    copie_plateau(pI, p);
    recherche_sokoban(p, &l, &col);

    for (int i = 0; i < tDep; i++) { // [cite: 13]
        usleep(TEMPS);
        if (traiter_entree(dep[i], &l, &col, &comp, pI, p)) {
            sJouee[nbJ++] = dep[i]; // 
        }
        afficher_entete(f_partie, comp);
        afficher_plateau(p);
    }

    if (gagne(p)) { // [cite: 14]
        int tOpti = 0;
        printf("\nSolution valide pour %s\n", f_partie);
        optimiser(sJouee, nbJ, sOpti, &tOpti); // [cite: 15, 29]
        printf("Initial : %d | Optimisé : %d\n", tDep, tOpti);
        printf("Enregistrer (O/N) ? "); char c; scanf(" %c", &c);
        if (c == 'O' || c == 'o') enregistrer(sOpti, tOpti); // [cite: 16]
    } else printf("\nLa suite N'EST PAS une solution\n"); // [cite: 23]
    return 0;
}

// --- Logique de mouvement et optimisation ---

bool deplacer(t_Plateau p, int *l, int *c, int *comp, char t, 
              t_Plateau pI, int cai) {
    int di, dj;
    if (!obtenir_direction(t, &di, &dj)) return false;
    int ti = *l + di, tj = *c + dj;
    if (!verifier_mouvement(p[ti][tj], cai)) return false;

    if (cai == 1) { // Pousser une caisse
        int bi = ti + di, bj = tj + dj;
        if (p[bi][bj] != C_VID && p[bi][bj] != C_CIB) return false;
        p[bi][bj] = (p[bi][bj] == C_CIB) ? C_CAI_C : C_CAI;
    }
    p[ti][tj] = (p[ti][tj] == C_CIB) ? C_SOK_C : C_SOK;
    p[*l][*c] = (pI[*l][*c] == C_CIB || pI[*l][*c] == C_CAI_C || 
                 pI[*l][*c] == C_SOK_C) ? C_CIB : C_VID;
    *l = ti; *c = tj; (*comp)++;
    return true;
}

void optimiser(t_tabDeplacement in, int tIn, t_tabDeplacement out, int *tOut) {
    int n = 0;
    for (int i = 0; i < tIn; i++) {
        out[n++] = in[i];
        if (n >= 2) { // [cite: 31]
            char c1 = out[n-2], c2 = out[n-1];
            bool sup = false;
            // Vérifie si deux déplacements simples s'annulent [cite: 29]
            if ((c1 == D && c2 == G) || (c1 == G && c2 == D)) sup = true;
            if ((c1 == H && c2 == B) || (c1 == B && c2 == H)) sup = true;
            if (sup) n -= 2; 
        }
    }
    *tOut = n;
}

bool traiter_entree(char t, int *l, int *c, int *comp, t_Plateau pI, 
                    t_Plateau p) {
    int cai = 0;
    if (t == H_C || t == G_C || t == B_C || t == D_C) {
        t = tolower(t);
        cai = 1;
    }
    return deplacer(p, l, c, comp, t, pI, cai);
}

// --- Utilitaires ---

void charger_partie(t_Plateau p, char f_nom[15]) {
    FILE *f = fopen(f_nom, "r");
    char fin;
    if (!f) exit(EXIT_FAILURE);
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) fread(&p[i][j], 1, 1, f);
        fread(&fin, 1, 1, f);
    }
    fclose(f);
}

void charger_deplacements(t_tabDeplacement t, char f_nom[], int *nb) {
    FILE *f = fopen(f_nom, "r");
    char d; *nb = 0;
    if (f) {
        while (fread(&d, 1, 1, f) && *nb < MOUVEMENT) t[(*nb)++] = d;
        fclose(f);
    }
}

void afficher_plateau(t_Plateau p) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            char c = p[i][j];
            if (c == C_CAI_C) printf("%c", C_CAI);
            else if (c == C_SOK_C) printf("%c", C_SOK);
            else printf("%c", c);
        }
        printf("\n");
    }
}

void afficher_entete(char f[], int c) {
    system("clear");
    printf("=== Sokoban ===\nPartie : %s\nCoups : %d\n", f, c);
}

bool gagne(t_Plateau p) {
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            if (p[i][j] == C_CAI) return false;
    return true;
}

void copie_plateau(t_Plateau p1, t_Plateau p2) {
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++) p2[i][j] = p1[i][j];
}

void recherche_sokoban(t_Plateau p, int *l, int *c) {
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            if (p[i][j] == C_SOK || p[i][j] == C_SOK_C) { *l = i; *c = j; }
}

bool obtenir_direction(char t, int *di, int *dj) {
    *di = (t == H) ? -1 : (t == B) ? 1 : 0;
    *dj = (t == G) ? -1 : (t == D) ? 1 : 0;
    return (*di != 0 || *dj != 0);
}

bool verifier_mouvement(char cible, int caisse) {
    if (cible == C_MUR) return false;
    return (caisse) ? (cible == C_CAI || cible == C_CAI_C) : 
                      (cible != C_CAI && cible != C_CAI_C);
}

void enregistrer(t_tabDeplacement s, int t) {
    char nf[20]; printf("Nom fichier : "); scanf("%s", nf);
    FILE *f = fopen(nf, "w");
    if (f) {
        for (int i = 0; i < t; i++) fputc(s[i], f);
        fclose(f);
    }
}