import numpy as np

# CAS 3 : La Figure 3
# Configurer le point de départ comme sur la figure 3 du sujet et trouver la solution (case A8)
# 
# Taille modifiable ligne 19
#
# Victor CORBEL - Temeio HARAPOI-GAUDIN / 1B1

tab = [['A1', 'B1', 'C1', 'D1', 'E1', 'F1', 'G1', 'H1'],
       ['A2', 'B2', 'C2', 'D2', 'E2', 'F2', 'G2', 'H2'],
       ['A3', 'B3', 'C3', 'D3', 'E3', 'F3', 'G3', 'H3'],
       ['A4', 'B4', 'C4', 'D4', 'E4', 'F4', 'G4', 'H4'],
       ['A5', 'B5', 'C5', 'D5', 'E5', 'F5', 'G5', 'H5'],
       ['A6', 'B6', 'C6', 'D6', 'E6', 'F6', 'G6', 'H6'],
       ['A7', 'B7', 'C7', 'D7', 'E7', 'F7', 'G7', 'H7'],
       ['A8', 'B8', 'C8', 'D8', 'E8', 'F8', 'G8', 'H8']]

taille = 8

# 0 = non visité, 1 = visité
echiquier = np.zeros((taille, taille), dtype=int)

# Les mouvements possibles du cavalier
mouvements = [(2, 1), (2, -1), (-2, 1), (-2, -1),
             (1, 2), (1, -2), (-1, 2), (-1, -2)]

def estValide(x: int, y: int):
    '''
    Vérifie si une position est valide (dans les limites et non visitée)
    '''
    if 0 <= x < taille and 0 <= y < taille:
        if echiquier[x][y] == 0:  # Si case non visitée
            return True
    return False

def dfs_cavalier(x: int, y: int, compteur: int, chemin: list, ferme: bool = False):
    '''
    DFS avec Backtracking pour résoudre le problème du cavalier.
    
    Paramètres:
    - x, y : position actuelle
    - compteur : nombre de cases visitées
    - chemin : liste des cases visitées dans l'ordre
    - ferme : booléen indiquant si le tour doit être fermé
    '''
    # Marquer la case actuelle comme visitée
    echiquier[x][y] = compteur
    chemin.append(tab[x][y])
    if ferme == False:
        # Condition d'arrêt : si toutes les cases sont visitées (8*8 = 64)
        if compteur == taille * taille:
            return True
        
        # Tester tous les mouvements possibles du cavalier
        coups = []
        for dx, dy in mouvements:
            nouveauX, nouveauY = x + dx, y + dy
            if estValide(nouveauX, nouveauY):
                # Compter le nombre de sorties possibles depuis la case
                sorties = 0
                for ddx, ddy in mouvements:
                    nx, ny = nouveauX + ddx, nouveauY + ddy
                    if estValide(nx, ny):
                        sorties += 1
                coups.append((sorties, nouveauX, nouveauY))

        # Prioriser les cases avec le moins de sorties possibles
        coups.sort(key=lambda c: c[0])

        for _, nouveauX, nouveauY in coups:
            # Appel récursif
            if dfs_cavalier(nouveauX, nouveauY, compteur + 1, chemin, ferme):
                return True  # Solution trouvée
    else:
        if compteur == taille * taille + 1:
            # Vérifier si le cavalier peut revenir à la position de départ
            for dx, dy in mouvements:
                if x + dx == chemin[0][0] and y + dy == chemin[0][1]:
                    return True
            return False
    
    # Backtracking : remet la case à 0 (non visitée) car impasse
    echiquier[x][y] = 0
    chemin.pop()
    return False

# Figure 3 : Lancer l'algorithme à partir de la case (1, 7)
taille = 8
chemin = []
if dfs_cavalier(0, 7, 1, chemin):
    print("Solution trouvée !")
    print(f"Chemin du cavalier ({len(chemin)} cases) :")
    print(chemin)
    print("\nMatrice de visite (ordre de passage) :")
    print(echiquier)
else:
    print("Aucune solution trouvée.")