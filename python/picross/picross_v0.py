grille = [[0, 1, 1, 0, 1], 
          [1, 0, 0, 1, 0], 
          [0, 1, 0, 0, 1], 
          [1, 0, 1, 0, 0], 
          [0, 1, 0, 1, 0]]
indices = [[2, 1], [1, 1], [1, 1], [1, 1], [1, 1]]





def est_valide(ligne, indice) -> bool:
    # prends une ligne (ex : [0, 1, 1, 0, 1]) et les indices associés (ex: [2, 1]) et retourne True si la ligne respecte les regles
    compteur_l = 0 #compteur pour les lignes
    compteur_i = 0 #compteur pour les indices
    for i in ligne:
        if i != 0: #si la case est remplie
            compteur_l += 1
        else: #si la case est vide
            if compteur_l != 0: #si on a compté des cases remplies
                if compteur_l != indice[compteur_i]: #si le nombre de cases remplies ne correspond pas à l'indice
                    return False
                compteur_i += 1 #on passe à l'indice suivant
                compteur_l = 0 #on réinitialise le compteur de lignes
    return True
