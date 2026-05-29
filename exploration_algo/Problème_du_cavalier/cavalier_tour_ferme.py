import numpy as np

# CAS 2 : Tour Fermé (Cycle)
# Modifier l'algorithme pour que la dernière case permette de sauter vers la première
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

depart_x = 0
depart_y = 0

def estValide(x: int, y: int):
    '''
    Vérifie si une position est valide (dans les limites et non visitée)
    '''
    if 0 <= x < taille and 0 <= y < taille:
        if echiquier[x][y] == 0:  # Si case non visitée
            return True
    return False

def dfs_cavalier(x: int, y: int, compteur: int, chemin: list, ferme: bool = False, profondeur: int = 0):
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
    if compteur == taille * taille:
        if ferme == False:
            return True
        for dx, dy in mouvements:
            if x + dx == depart_x and y + dy == depart_y:
                return True
        echiquier[x][y] = 0
        chemin.pop()
        return False

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

    if profondeur > 5:
        coups = coups[:2] # On ne garde que les 2 meilleurs coups pour limiter la profondeur de l'arbre

    for _, nouveauX, nouveauY in coups:
        # Appel récursif
        if dfs_cavalier(nouveauX, nouveauY, compteur + 1, chemin, ferme, profondeur + 1):
            return True  # Solution trouvée

    # Backtracking : remet la case à 0 (non visitée) car impasse
    echiquier[x][y] = 0
    chemin.pop()
    return False

chemin = []
depart_x = 0
depart_y = 0
if dfs_cavalier(depart_x, depart_y, 1, chemin, True):
    print("Solution trouvée !")
    print(f"Chemin du cavalier ({len(chemin)} cases) :")
    print(chemin)
    print("\nMatrice de visite (ordre de passage) :")
    print(echiquier)
else:
    print("Aucune solution trouvée.")