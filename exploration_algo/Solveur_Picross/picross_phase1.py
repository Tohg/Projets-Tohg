# Solveur de Picross
# Phase 1 : Modélisation et Vérification
#
# Fonction est_valide(ligne, indices)
#
# Victor CORBEL - Temeio HARAPOI-GAUDIN / 1B1

grille = [[0, 1, 1, 0, 1], 
          [1, 0, 0, 1, 0], 
          [0, 1, 0, 0, 1], 
          [1, 0, 1, 0, 0], 
          [0, 1, 0, 1, 0]]
indices = [[2, 1], [1, 1], [1, 1], [1, 1], [1, 1]]

def est_valide(ligne, indice) -> bool:
    """
    Vérifie si une ligne respecte les indices donnés.
    ligne : liste de 0 et 1 représentant une ligne de la grille.
    indice : liste d'entiers représentant les blocs attendus.
    Retourne True si la ligne respecte les indices, False sinon.
    """
    # Prend une ligne (ex : [0, 1, 1, 0, 1]) et les indices associés (ex: [2, 1]) et retourne True si la ligne respecte les regles
    compteur_l = 0 # Compteur pour les lignes
    compteur_i = 0 # Compteur pour les indices
    for i in ligne:
        if i != 0: # Si la case est remplie
            compteur_l += 1
        else: # Si la case est vide
            if compteur_l != 0: # Si on a compté des cases remplies
                if compteur_i >= len(indice) or compteur_l != indice[compteur_i]: # Si le nombre de cases remplies ne correspond pas à l'indice
                    return False
                compteur_i += 1 # On passe à l'indice suivant
                compteur_l = 0 # On réinitialise le compteur de lignes
    if compteur_l != 0:
        if compteur_i >= len(indice) or compteur_l != indice[compteur_i]:
            return False
        compteur_i += 1
    return compteur_i == len(indice)

if __name__ == "__main__":
    print(est_valide([0, 1, 1, 0, 1], [2, 1]))
    print(est_valide([1, 0, 0, 1, 0], [1, 1]))
    print(est_valide([0, 1, 0, 0, 1], [1, 1]))
    print(est_valide([1, 0, 1, 0, 0], [1, 1]))
    print(est_valide([0, 1, 0, 1, 0], [1, 1]))