import picross_phase1

est_valide = picross_phase1.est_valide

def peut_etre_valide(partial_line, indice, total_length):
    """
    Vérifie si une ligne partielle peut encore être complétée pour respecter les indices.
    partial_line: la partie déjà remplie de la ligne
    indice: les indices requis
    total_length: la longueur totale de la ligne
    """
    # Compter les blocs complets (séparés par des 0)
    blocs_complets = []
    compteur = 0
    for cell in partial_line:
        if cell == 1:
            compteur += 1
        elif compteur > 0:
            blocs_complets.append(compteur)
            compteur = 0
    
    # Le dernier bloc peut encore grandir si la ligne ne se termine pas par 0
    dernier_est_ouvert = partial_line and partial_line[-1] == 1
    
    if dernier_est_ouvert:
        blocs_complets.append(compteur)
    
    # Vérifier si les blocs complets correspondent au début des indices
    if len(blocs_complets) > len(indice):
        return False
    
    for i, bloc in enumerate(blocs_complets):
        if i >= len(indice):
            return False
        if not dernier_est_ouvert or i < len(blocs_complets) - 1:
            # Blocs complets doivent correspondre exactement
            if bloc != indice[i]:
                return False
        else:
            # Le dernier bloc (ouvert) peut encore grandir
            if bloc > indice[i]:
                return False
    
    # Vérifier les blocs restants
    blocs_restants = indice[len(blocs_complets):]
    if not blocs_restants:
        # Plus de blocs requis, vérifier qu'il n'y a pas de 1s restants
        cellules_restantes = total_length - len(partial_line)
        return cellules_restantes >= 0  # Pas de problème si on peut remplir avec 0s
    
    # Calculer l'espace disponible pour les blocs restants
    cellules_restantes = total_length - len(partial_line)
    cellules_occupees = sum(blocs_complets) + (len(blocs_complets) - (1 if dernier_est_ouvert else 0))
    
    # Espace minimum nécessaire: somme des blocs + espaces entre eux (au moins 1 entre chaque paire)
    espace_min = sum(blocs_restants)
    if len(blocs_restants) > 1:
        espace_min += len(blocs_restants) - 1  # espaces entre blocs
    
    return cellules_restantes >= espace_min

def backtracking(indice_l, indice_c, ligne=0, colonne=0) -> list[list[int]]:
    """
    Résout le Picross en utilisant un véritable algorithme de backtracking.
    """
    n = len(indice_l)  # nombre de lignes
    m = len(indice_c)  # nombre de colonnes
    
    solution = [[0 for _ in range(m)] for _ in range(n)]
    
    def solve(pos):
        """
        Fonction récursive pour résoudre le puzzle.
        pos est la position linéaire (ligne * m + colonne)
        """
        if pos == n * m:
            # Vérification finale de toutes les lignes et colonnes
            for i in range(n):
                if not est_valide(solution[i], indice_l[i]):
                    return False
            for j in range(m):
                col = [solution[i][j] for i in range(n)]
                if not est_valide(col, indice_c[j]):
                    return False
            return True
        
        i, j = divmod(pos, m)
        
        # Essayer 0
        solution[i][j] = 0
        # Vérifier la ligne partielle
        ligne_partielle = solution[i][:j+1]
        if peut_etre_valide(ligne_partielle, indice_l[i], m):
            # Vérifier la colonne partielle
            col_partielle = [solution[k][j] for k in range(i+1)]
            if peut_etre_valide(col_partielle, indice_c[j], n):
                if solve(pos + 1):
                    return True
        
        # Essayer 1
        solution[i][j] = 1
        # Vérifier la ligne partielle
        ligne_partielle = solution[i][:j+1]
        if peut_etre_valide(ligne_partielle, indice_l[i], m):
            # Vérifier la colonne partielle
            col_partielle = [solution[k][j] for k in range(i+1)]
            if peut_etre_valide(col_partielle, indice_c[j], n):
                if solve(pos + 1):
                    return True
        
        # Aucun choix ne fonctionne, backtrack
        return False
    
    if solve(0):
        return solution
    else:
        return None  # Pas de solution trouvée


indices_lignes = [[1,1,1,3],[3,1,1],[1,2,2],[1,2,1,2],[3,2],[1,2],[1,2,1],[2,3,1],[10],[1,3]]
indices_colonnes = [[1,2,2],[1,1,1,2],[1,2,3],[2,2,1,1],[2,2],[1,2,1,3],[1,5],[1,2],[1,3,1],[5,2]]

solution = backtracking(indices_lignes, indices_colonnes)
if solution:
    print("Solution trouvée:")
    for ligne in solution:
        print(ligne)
else:
    print("Aucune solution trouvée")